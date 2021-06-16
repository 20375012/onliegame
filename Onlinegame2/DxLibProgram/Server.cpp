
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Server.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		サーバーアプリケーション起動時の処理
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- 必要ファイルのインクルード
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"      // --- ゲーム共通のヘッダーファイル

/* ===============================================================================
|
|   Server_Game_Main
|       ゲームメイン関数
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|   OUT --> int                 --> 終了値
+ ============================================================================= */
int Server_Game_Main( HWND hWnd )
{
	// --- ゲームモード分岐
	switch( game_mode )
	{
		case GM_S_WAIT:		GM_Server_Wait	( );		break;	// --- クライアント待ち
		case GM_S_START:	GM_Server_Start	( );		break;	// --- スタート
		case GM_S_PLAY:		GM_Server_Play	( );		break;	// --- 対戦中
		case GM_S_END:		GM_Server_End	( hWnd );	break;	// --- 終了
	}

	// --- サーバー常時処理	
//	ServerToClient_Send	( );		// --- 送信処理
//	Server_Disp_Show	( hWnd );	// --- 描画処理

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Wait
|       サーバークライアント待ち
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Server_Wait( void )
{	
	// --- アドレスが格納されていたら
	if ( recv_addr.sin_addr.s_addr != 0 )
	{
		game_mode = GM_S_START;			// --- ゲーム開始
		server_sned_work.mode = 1;		// --- ゲーム開始データ格納
	}

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Start
|       サーバー開始時処理
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Server_Start( void )
{	
	// --- 先攻後攻決定
	turn_val = 0;//rand( ) % 2;

	// --- ターン判定
	if ( turn_val == 0 )
	{
		play_mode[ PLYER_1P ] = GM_S_PLAY_TURN;		
		play_mode[ PLYER_2P ] = GM_S_PLAY_WAIT;	
		turn_val = -1;	
	}
	else if ( turn_val == 1 )
	{
		play_mode[ PLYER_1P ] = GM_S_PLAY_WAIT;		
		play_mode[ PLYER_2P ] = GM_S_PLAY_TURN;		
		turn_val = -1;	
	}

	// --- 最初は-1にしておく
	server_recv_work.mouse_point.x = server_recv_work.mouse_point.y = -1;

	game_mode = GM_S_PLAY;

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Play
|       サーバー対戦中時処理
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Server_Play( void )
{
	GM_Server_Play_1P( );	// --- サーバー1P処理
	GM_Server_Play_2P( );	// --- サーバー2P処理

	// --- ターン判定
	if ( turn_val == 0 )
	{
		// --- パス判定
		if ( Pass_Check( OSERO_CHIP_BLACK, OSERO_CHIP_WHITE ) == TRUE )
		{
			play_mode[ PLYER_1P ] = GM_S_PLAY_TURN;		
			play_mode[ PLYER_2P ] = GM_S_PLAY_WAIT;		
		}
		turn_val = -1;	
	}
	else if ( turn_val == 1 )
	{
		// --- パス判定
		if ( Pass_Check( OSERO_CHIP_WHITE, OSERO_CHIP_BLACK ) == TRUE )
		{
			play_mode[ PLYER_1P ] = GM_S_PLAY_WAIT;		
			play_mode[ PLYER_2P ] = GM_S_PLAY_TURN;	
		}
		turn_val = -1;	
	}

	// --- チップ数取得
	chip_val[ PLYER_1P ] = Chip_Val_Check( OSERO_CHIP_BLACK );
	chip_val[ PLYER_2P ] = Chip_Val_Check( OSERO_CHIP_WHITE );

	// --- メッセージ取得
	if ( ( key_state[ KEY_F1 ] < 0 ) && GetWindowTextLength( edit_window ) != 0 )
	{
		GetWindowText( edit_window, message_str, sizeof( message_str ) );	// --- メッセージの取得
		SetWindowText( edit_window, "" );									// --- メッセージのセット
	}

	// --- メッセージ表示
	SetWindowText( static_window, server_recv_work.message_str );	// --- 相手のメッセージのセット

	// --- 終了判定
	if ( Finalize_Check( ) == TRUE )
	{
		server_sned_work.mode = 2;		// --- ゲーム終了データ格納
		game_mode = GM_S_END;
	}

	return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Play_1P
|       サーバー1P処理
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Server_Play_1P( void )
{
	if ( play_mode[ PLYER_1P ] == GM_S_PLAY_TURN )
	{
		// --- マウスの左が押されていたら
		if ( key_state[ KEY_MOUSE_L ] < 0 )
		{
			// --- 配列座標を算出
			mouse_point.x = ( mouse_point.x - DISP_BOARD_X ) / ( SPR_CHIP_CUT_W + CHIP_ADJUST_X );
			mouse_point.y = ( mouse_point.y - DISP_BOARD_Y ) / ( SPR_CHIP_CUT_H + CHIP_ADJUST_Y );

			// --- リミットチェック
			if ( mouse_point.x < OSERO_X_MAX && mouse_point.y < OSERO_Y_MAX && mouse_point.x >= 0 && mouse_point.y >= 0 )
			{
				if ( Chip_Set_Check( mouse_point, OSERO_CHIP_BLACK, OSERO_CHIP_WHITE ) == TRUE )
				{
					turn_val = 1;	// --- ターンチェンジ
				}
			}
		}
	}

	return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Play_2P
|       サーバー2P処理
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Server_Play_2P( void )
{
	if ( play_mode[ PLYER_2P ] == GM_S_PLAY_TURN )
	{
		if ( server_recv_work.mouse_point.x != -1 )
		{
			if ( Chip_Set_Check( server_recv_work.mouse_point, OSERO_CHIP_WHITE, OSERO_CHIP_BLACK ) == TRUE )
			{
				turn_val = 0;	// --- ターンチェンジ
			}
		}
	}
	
    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_End
|       サーバー終了処理
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Server_End( HWND hWnd )
{
	char str[ 256 ];

	if ( black_cnt > white_cnt )		// --- 黒の勝利
	{
		wsprintf( str, " %d 対 %d で黒の勝利です。\n OKボタンでアプリケーションを終了します。", black_cnt, white_cnt );
	}
	else if ( black_cnt < white_cnt )	// --- 白の勝利
	{
		wsprintf( str, " %d 対 %d で白の勝利です。\n OKボタンでアプリケーションを終了します。", black_cnt, white_cnt );
	}
	else								// --- 引き分け
	{
		wsprintf( str, " %d 対 %d で引き分けです。\n OKボタンでアプリケーションを終了します。", black_cnt, white_cnt );
	}

	game_mode++;
	MessageBox( hWnd, "終了", "Result", MB_OK );	
    return( TRUE );
}

/* ===============================================================================
|
|   Server_Disp_Show
|       サーバー描画処理
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|   OUT --> int                 --> 終了値
+ ============================================================================= *
int Server_Disp_Show( HWND hWnd )
{
	HDC	hDC;
	int i;
	//int val1, val2;

	hDC = CreateCompatibleDC( NULL );

	// --- 背景を描画
    SelectObject( hDC, hBitmapSpr[ 0 ] );
	BitBlt( game_win.hScreenDC, 0, 0, game_win.win_size.cx, game_win.win_size.cy, hDC, 0, 0, SRCCOPY ) ;

	// --- 盤上のチップを描画
	SelectObject( hDC, hBitmapSpr[ 1 ] );
	for ( i = 0 ; i < OSERO_X_MAX * OSERO_Y_MAX ; i++ )
	{	
		if ( osero_board_array[ i ] != OSERO_CHIP_NON )
		{
			TransparentBlt
			( 
				game_win.hScreenDC, 
				( ( i % OSERO_X_MAX ) * SPR_CHIP_CUT_W ) + DISP_BOARD_X + ( ( i % OSERO_X_MAX ) * CHIP_ADJUST_X ),
				( ( i / OSERO_Y_MAX ) * SPR_CHIP_CUT_H ) + DISP_BOARD_Y + ( ( i / OSERO_X_MAX ) * CHIP_ADJUST_Y ), 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				hDC, 
				( osero_board_array[ i ] - 1 ) * SPR_CHIP_CUT_W, 0, 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				RGB( 0, 255, 0 ) 
			);
		}
	}
/*
	// --- 数値をセット
	SelectObject( hDC, hBitmapSpr[ 2 ] );

	// --- PLAYER1のチップ数表示
	val1 = ( chip_val[ PLYER_1P ] / 10 ) % 10;	// --- 十の位
	val2 = ( chip_val[ PLYER_1P ] / 1 ) % 10;	// --- 一の位

	TransparentBlt
	( 
		game_win.hScreenDC, 
		56, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val1 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);
	TransparentBlt
	( 
		game_win.hScreenDC, 
		88, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val2 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);

	// --- PLAYER2のチップ数表示
	val1 = ( chip_val[ PLYER_2P ] / 10 ) % 10;	// --- 十の位
	val2 = ( chip_val[ PLYER_2P ] / 1 ) % 10;	// --- 一の位

	TransparentBlt
	( 
		game_win.hScreenDC, 
		520, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val1 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);
	TransparentBlt
	( 
		game_win.hScreenDC, 
		552, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val2 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);
*//*
	DeleteDC( hDC ); 
	
    return( TRUE );
}

/* ===============================================================================
|
|   ServerToClient_Send
|       サーバーからクライアントへ送信
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= *
int ServerToClient_Send( void )
{
	// --- データの作成
	memcpy( server_sned_work.osero_board_array, osero_board_array, sizeof( osero_board_array ) );	// --- 盤面をコピー
	memcpy( server_sned_work.chip_val, chip_val, sizeof( chip_val ) );								// --- チップ数情報
	memcpy( server_sned_work.message_str, message_str, sizeof( message_str ) );						// --- メッセージ文字列
	server_sned_work.non_cnt	= non_cnt;
	server_sned_work.black_cnt	= black_cnt;
	server_sned_work.white_cnt	= white_cnt;

	// --- クライアントに送信
	sendto( sock, ( const char * )&server_sned_work, sizeof( SERVER_SEND_DATA ), 0, ( struct sockaddr * )&recv_addr, sizeof( recv_addr ) );	
	
    return( TRUE );
}*/
