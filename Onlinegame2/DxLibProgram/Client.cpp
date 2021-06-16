
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Client.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		クライアントアプリケーション起動時の処理
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- 必要ファイルのインクルード
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"      // --- ゲーム共通のヘッダーファイル

/* ===============================================================================
|
|   Client_Game_Main
|       ゲームメイン関数
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|   OUT --> int                 --> 終了値
+ ============================================================================= */
int Client_Game_Main( HWND hWnd )
{
	// --- ゲームモード分岐
	switch( game_mode )
	{
		case GM_C_FIND:		GM_Client_Find	( );	break;	// --- サーバー探索
		case GM_C_START:	GM_Client_Start	( );	break;	// --- スタート
		case GM_C_PLAY:		GM_Client_Play	( );	break;	// --- 対戦中
		case GM_C_END:		GM_Client_End	( );	break;	// --- 終了
	}

	// --- クライアント常時処理	
//	ClientToServer_Send	( );		// --- 送信処理
//	Client_Disp_Show	( hWnd );	// --- 描画処理

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Client_Find
|       サーバー探索
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Client_Find( void )
{	
	// --- サーバーに送信後、開始を待つ
	if ( free_mode == 0 )
	{
		client_sned_work.mode = 1;	// --- サーバーに通達
		free_mode++;
	}
	else
	{
		// --- サーバーから開始要請が到着していたら
		if ( client_recv_work.mode == 1 )
		{
			free_mode = 0;
			game_mode = GM_C_START;
		}
	}

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Client_Start
|       クライアント開始時処理
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Client_Start( void )
{	
	// --- 最初は-1にしておく
	client_sned_work.mouse_point.x = -1;
	client_sned_work.mouse_point.y = -1;

	game_mode = GM_C_PLAY;

    return( TRUE );
}


/* ===============================================================================
|
|   GM_Client_End
|       クライアント終了処理
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= */
int GM_Client_End( void )
{
	char str[ 256 ];

	if ( client_recv_work.black_cnt > client_recv_work.white_cnt )		// --- 黒の勝利
	{
		wsprintf( str, " %d 対 %d で黒の勝利です。\n OKボタンでアプリケーションを終了します。", client_recv_work.black_cnt, client_recv_work.white_cnt );
	}
	else if ( client_recv_work.black_cnt < client_recv_work.white_cnt )	// --- 白の勝利
	{
		wsprintf( str, " %d 対 %d で白の勝利です。\n OKボタンでアプリケーションを終了します。", client_recv_work.black_cnt, client_recv_work.white_cnt );
	}
	else	// --- 引き分け
	{
		wsprintf( str, " %d 対 %d で引き分けです。\n OKボタンでアプリケーションを終了します。", client_recv_work.black_cnt, client_recv_work.white_cnt );
	}

	game_mode++;
	MessageBox( NULL, str, "Result", MB_OK );	
	
    return( TRUE );
}

/* ===============================================================================
|
|   Client_Disp_Show
|       クライアント描画処理
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|   OUT --> int                 --> 終了値
+ ============================================================================= *
int Client_Disp_Show( HWND hWnd )
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
		if ( client_recv_work.osero_board_array[ i ] != OSERO_CHIP_NON )
		{
			TransparentBlt
			( 
				game_win.hScreenDC, 
				( ( i % OSERO_X_MAX ) * SPR_CHIP_CUT_W ) + DISP_BOARD_X + ( ( i % OSERO_X_MAX ) * CHIP_ADJUST_X ),
				( ( i / OSERO_Y_MAX ) * SPR_CHIP_CUT_H ) + DISP_BOARD_Y + ( ( i / OSERO_X_MAX ) * CHIP_ADJUST_Y ), 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				hDC, 
				( client_recv_work.osero_board_array[ i ] - 1 ) * SPR_CHIP_CUT_W, 0, 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				RGB( 0, 255, 0 ) 
			);
		}
	}
/*
	// --- 数値をセット
	SelectObject( hDC, hBitmapSpr[ 2 ] );

	// --- PLAYER1のチップ数表示
	val1 = ( client_recv_work.chip_val[ PLYER_1P ] / 10 ) % 10;	// --- 十の位
	val2 = ( client_recv_work.chip_val[ PLYER_1P ] / 1 ) % 10;	// --- 一の位

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
	val1 = ( client_recv_work.chip_val[ PLYER_2P ] / 10 ) % 10;	// --- 十の位
	val2 = ( client_recv_work.chip_val[ PLYER_2P ] / 1 ) % 10;	// --- 一の位

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
|   ClientToServer_Send
|       クライアントからサーバーへ送信
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> 終了値
+ ============================================================================= *
int ClientToServer_Send( void )
{
	// --- データの作成
	memcpy( client_sned_work.message_str, message_str, sizeof( message_str ) );						// --- メッセージ文字列

	// --- サーバーに送信
	sendto( sock, ( const char * )&client_sned_work, sizeof( CLIENT_SEND_DATA ), 0, ( struct sockaddr * )&addr, sizeof( addr ) );	
	
    return( TRUE );
}


/* ===============================================================================
|
|   
|   
+ --------------------------------------------------------------------------------
|  
|  
+ ============================================================================= */
int GM_Client_Play(void)
{

	if (key_state[KEY_MOUSE_L] < 0) {
		mouse_point.x = (mouse_point.x - DISP_BOARD_X) / 
						(SPR_CHIP_CUT_W + CHIP_ADJUST_X);
		mouse_point.y = (mouse_point.y - DISP_BOARD_Y) / 
						(SPR_CHIP_CUT_H + CHIP_ADJUST_Y);

		if(mouse_point.x < OSERO_X_MAX && mouse_point.y < OSERO_Y_MAX &&
									mouse_point.x >= 0 && mouse_point.y >= 0) {

			client_sned_work.mouse_point.x = mouse_point.x;
			client_sned_work.mouse_point.y = mouse_point.y;
		}
		else
		{
			client_sned_work.mouse_point.x = -1;
			client_sned_work.mouse_point.y = -1;
		}

	}
	else
	{
		client_sned_work.mouse_point.x = -1;
		client_sned_work.mouse_point.y = -1;
	}

	if((key_state[KEY_F1] < 0) && GetWindowTextLength(edit_window) != 0) {

		GetWindowText( edit_window, message_str,sizeof(message_str) );
		SetWindowText( edit_window, "" );
	}

	SetWindowText( static_window,client_recv_work.message_str );

	if (client_recv_work.mode == 2) {
		game_mode = GM_C_END;
	}

	return(TRUE);

}





