
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Common.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		データやサーバー&クライアント共通での処理、または汎用性がある関数の処理
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- 必要ファイルのインクルード
#include "WINSOCK2.h"
#include "windows.h"
#include <stdio.h>	// --- コンソール用

#include "Common.h"      // --- ゲーム共通のヘッダーファイル

/* -----------------------------------------------------------------------------------------
|
|
|       データ宣言
|
|
+ --------------------------------------------------------------------------------------- */
/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|       テーブルデータ
+ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
// --- オセロの盤面データ
int osero_board_array[ OSERO_X_MAX * OSERO_Y_MAX ] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};


/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|       変数、構造体データ
+ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
int				app_mode;						// --- アプリケーションモード
int				game_mode;						// --- ゲームモード
int				free_mode;						// --- フリーモード

int				play_mode[ PLYER_MAX ];			// --- プレイ中のモード
int				chip_val[ PLYER_MAX ];			// --- 各プレイヤーチップ数
int				turn_val;						// --- ターン判定番号
char			message_str[ MESSAGE_STR_MAX ];	// --- メッセージ文字列
int				non_cnt, black_cnt, white_cnt;	// --- 各チップのカウンタ

int				key_state[ KEY_MAX ];			// --- キー状態
ConsoleWindow g_cWin;

POINT			mouse_point;					// --- マウスカーソル位置
HBITMAP			hBitmap;
HBITMAP			hBitmapSpr[ SPR_MAX ];
HWND	        edit_window; 
HWND		    static_window; 
GAMEWINDOW		game_win =
{
    NULL,
    NULL,
    { CLIENT_WIDTH, CLIENT_HEIGHT },
};


// --- コマンドライン引数
int				argc;
char 			**argv;

// --- 通信関連データ
SERVER_SEND_DATA	server_sned_work;				// --- サーバー送信データ
CLIENT_RECV_DATA	client_recv_work;				// --- クライアント受信データ
SERVER_RECV_DATA	server_recv_work;				// --- サーバー受信データ
CLIENT_SEND_DATA	client_sned_work;				// --- クライアント送信データ


WORD                version = MAKEWORD( 2, 0 );     // --- WinSockへの要求バージョン
WSADATA             ws_data;                        // --- WinSockのデータ
SOCKET              sock;							// --- サーバー＆クライアントソケット
struct sockaddr_in  addr;							// --- サーバー＆クライアントアドレス情報
struct sockaddr_in  recv_addr;						// --- レシーブ用
int					addr_len;						// --- レシーブ用アドレスサイズ

/* ===============================================================================
|
|   Game_Main_Function
|       ゲームメイン関数
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|           UINT message        --> メッセージ
|           WPARAM wParam       --> メッセージ補足情報
|           LPARAM lParam       --> メッセージ補足情報
|   OUT --> LRESULT             --> 終了値
+ ============================================================================= */
int Game_Main_Function( HWND hWnd )
{
	// --- キー取得
	key_state[ KEY_F1 ]			= GetKeyState( VK_F1 );
	key_state[ KEY_MOUSE_L ]	= GetKeyState( VK_LBUTTON );

	// --- マウスポジション取得
	GetCursorPos( &mouse_point );
	ScreenToClient( hWnd, &mouse_point );

    // --- サーバーかクライアントで分岐
    switch( app_mode )
    {
        case SERVER_APP: Server_Game_Main( hWnd ); break;     // --- サーバゲームメイン処理
        case CLIENT_APP: Client_Game_Main( hWnd ); break;     // --- クライアントゲームメイン処理
    }
    return( TRUE );
}

/* ===============================================================================
|
|   Finalize_Check
|       終了をチェック
+ --------------------------------------------------------------------------------
|   IN ---> int		color1		--> 自分の色
|			int		color2		--> 相手の色
|   OUT --> int --> 終了値
+ ============================================================================= */
int Finalize_Check( void )
{
	int	i;

	// --- 盤面上を全てチェック
	black_cnt = white_cnt = non_cnt = 0;
	for ( i = 0 ; i < OSERO_X_MAX * OSERO_Y_MAX ; i++ )
	{
		if ( osero_board_array[ i ] == OSERO_CHIP_NON )		{ non_cnt++; }		// --- 置かれていない所をカウント
		if ( osero_board_array[ i ] == OSERO_CHIP_BLACK )	{ black_cnt++; }	// --- 黒の数をカウント
		if ( osero_board_array[ i ] == OSERO_CHIP_WHITE )	{ white_cnt++; }	// --- 白の数をカウント
	}

	// --- 終了条件をチェック
	if ( black_cnt != 0 && white_cnt == 0 )	{ return( TRUE ); }	// --- 黒だけだったら
	if ( black_cnt == 0 && white_cnt != 0 )	{ return( TRUE ); }	// --- 白だけだったら
	if ( non_cnt == 0 )						{ return( TRUE ); }	// --- 全部埋まっていたら

	return( FALSE );	// --- 継続
}

/* ===============================================================================
|
|   Pass_Check
|       パス状態をチェック
+ --------------------------------------------------------------------------------
|   IN ---> int		color1		--> 自分の色
|			int		color2		--> 相手の色
|   OUT --> int --> 終了値
+ ============================================================================= */
int Pass_Check( int color1, int color2 )
{
	int		i;
	int		flg = 0;
	POINT	point;
	
	// --- 盤面上を全てチェック
	for ( i = 0 ; i < ( OSERO_X_MAX * OSERO_Y_MAX ) ; i++ )
	{
		// --- 空いていたら
		if ( osero_board_array[ i ] == OSERO_CHIP_NON )
		{
			// --- ポイント格納
			point.x = i % OSERO_X_MAX;
			point.y = i / OSERO_X_MAX;
		
			// --- 相手のチップが取れるかを判定
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 0, 0, FALSE );	// --- 上方向検索開始
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 1, 0, FALSE );	// --- 右方向検索開始
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 2, 0, FALSE );	// --- 下方向検索開始
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 3, 0, FALSE );	// --- 左方向検索開始
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 4, 0, FALSE );	// --- 左上方向検索開始
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 5, 0, FALSE );	// --- 左上方向検索開始
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 6, 0, FALSE );	// --- 右下方向検索開始
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 7, 0, FALSE );	// --- 左下方向検索開始

			// --- 一つでも取れていたら
			if ( flg == 1 )
			{
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

/* ===============================================================================
|
|   Chip_Set_Check
|       チップが置けるかの判定
+ --------------------------------------------------------------------------------
|   IN ---> POINT	dest_point	--> 要求されたポイント
|			int		color1		--> 自分の色
|			int		color2		--> 相手の色
|   OUT --> int --> 終了値
+ ============================================================================= */
int Chip_Set_Check( POINT dest_point, int color1, int color2 )
{
	int flg = 0;

	// --- 置いたと仮定した盤面データ
	int osero_board_virtual_array[ OSERO_X_MAX * OSERO_Y_MAX ];

	// --- 現状の盤面データのコピー
	memcpy( osero_board_virtual_array, osero_board_array, sizeof( osero_board_array ) );

	// --- 盤面上の指定箇所が空いていたら
	if ( osero_board_array[ dest_point.x + ( dest_point.y * OSERO_X_MAX ) ] == OSERO_CHIP_NON )
	{
		osero_board_virtual_array[ dest_point.x + ( dest_point.y * OSERO_X_MAX ) ] = color1;	// --- 自分の色を置く

		// --- 相手のチップが取れるかを判定
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 0, 0, TRUE );	// --- 上方向検索開始
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 1, 0, TRUE );	// --- 右方向検索開始
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 2, 0, TRUE );	// --- 下方向検索開始
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 3, 0, TRUE );	// --- 左方向検索開始
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 4, 0, TRUE );	// --- 左上方向検索開始
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 5, 0, TRUE );	// --- 左上方向検索開始
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 6, 0, TRUE );	// --- 右下方向検索開始
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 7, 0, TRUE );	// --- 左下方向検索開始
	}

	// --- 1つでも相手のチップが取れたら
	if ( flg == 1 )	
	{
		// --- 現状の盤面データのコピー
		memcpy( osero_board_array, osero_board_virtual_array, sizeof( osero_board_array ) );
		return( 1 ); 
	}
	else
	{
		return( 0 ); 
	}	
}

/* ===============================================================================
|
|	Chip_Chain_Reflexive_Func
|       相手のチップを取る処理(再起関数)
+ --------------------------------------------------------------------------------
|   IN ---> POINT	point		--> チェックポイント
|			int		*array		--> 盤面のポインタ
|			int		color1		--> 自分の色
|			int		color2		--> 相手の色
|			int		direction	--> 検索方向
|			int		cnt			--> 現在の処理数
|			int		option		--> オプション
|   OUT --> int --> 終了値
+ ============================================================================= */
int Chip_Chain_Reflexive_Func( POINT point, int *array, int color1, int color2, int direction, int cnt, int option )
{
	// --- 引数で指定された方向にインクリメント、デクリメント
	if ( direction == 0 ) { point.y--; }			// --- 上方向
	if ( direction == 1 ) { point.x++; }			// --- 右方向
	if ( direction == 2 ) { point.y++; }			// --- 下方向
	if ( direction == 3 ) { point.x--; }			// --- 左方向
	if ( direction == 4 ) { point.x--; point.y--; }	// --- 左上方向
	if ( direction == 5 ) { point.x++; point.y--; }	// --- 右上方向
	if ( direction == 6 ) { point.x++; point.y++; }	// --- 右下方向
	if ( direction == 7 ) { point.x--; point.y++; }	// --- 左下方向

	cnt++;	// --- 処理回数インクリメント

	// --- リミットチェック
	if ( point.x < OSERO_X_MAX && point.y < OSERO_Y_MAX && point.x >= 0 && point.y >= 0 )
	{
		// --- 指定されたポイントをチェック
		if ( array[ point.x + ( point.y * OSERO_X_MAX ) ] == color1 )		// --- 自分の色
		{
			// --- 隣じゃなかったら成立コード発行
			if ( cnt > 1 )	{ return( 1 ); }		// --- 成立コード
			else			{ return( 0 ); }	// --- 不成立コード
		}
		else if ( array[ point.x + ( point.y * OSERO_X_MAX ) ] == color2 )	// --- 相手の色
		{
			// --- 処理を続行し返り値を判定してテーブルを書き直す
			if ( Chip_Chain_Reflexive_Func( point, array, color1, color2, direction, cnt, option ) == TRUE )	// --- 自分の色に書き直す
			{
				if ( option == TRUE ) { array[ point.x + ( point.y * OSERO_X_MAX ) ] = color1; }
				return( 1 );	// --- 成立コード
			}
			else	// --- 自分の色にはできなかった
			{
				return( 0 );	// --- 不成立コード
			}
		}
		else	// --- 何も無かったら
		{
			return( 0 );	// --- 不成立コード
		}
	}
	else	// --- 範囲外だったら終了
	{
		return( 0 );	// --- 不成立コード
	}
}

/* ===============================================================================
|
|   Chip_Val_Check
|       指定のチップを数える
+ --------------------------------------------------------------------------------
|   IN ---> int	color1	--> 指定の色
|   OUT --> int --> 終了値
+ ============================================================================= */
int Chip_Val_Check( int color )
{
	int val = 0;
	int i;

	// --- 特定の数値を探す
	for ( i = 0 ; i < OSERO_X_MAX * OSERO_Y_MAX ; i++ )
	{
		if ( osero_board_array[ i ] == color ) { val++; }
	}
	return( val );	// --- 結果を返す
}

/* =======================================================================
|
|   Free_Args
|       引数リスト解放関数
+ ------------------------------------------------------------------------
|   IN ---> int		argc        --> コマンドライン数
|           char	**argv      --> コマンドラインリスト
|   OUT --> LRESULT             --> 終了値
+ ===================================================================== */
void Free_Args( int argc, char **argv )
{
    int i;

    for( i = 0 ; i < argc ; i++ ) free( argv[ i ] );

    free( argv );
}

/* =======================================================================
|
|   Get_Args
|       引数リスト取得関数
+ ------------------------------------------------------------------------
|   IN ---> int		argc        --> コマンドライン数
|           char	**argv      --> コマンドラインリスト
|   OUT --> LRESULT             --> 終了値
+ ===================================================================== */
int Get_Args( const char *cmdline, char ***pargv ) 
{
    int len, i, ct = 0, mct = 0, cch = 0, mcch = 0, dq = 0;
    
	char *tmpb = NULL, *tmpbn, **argv, **argvn;

    len = strlen( cmdline );
    for( i = 0 ; i <= len ; i++ )  
	{
        if( cmdline[ i ] == '\"' )  
		{
            if( dq )
			{
                if( cmdline[ i + 1 ] == ' ' )  dq = 0;
            } 
			else
			{ 
				dq = 1;
			}
        } 
		else if( ( ( !dq ) && ( cmdline[ i ] == ' ' ) ) || ( i == len ) )  
		{
            if( cch||dq||( i ? ( cmdline[ i - 1 ] == '\"' ) : 0 ) )  
			{
                if( ct == mct )  
				{
                    mct += 16;
                    argvn = ( char ** )calloc( mct, sizeof( char * ) );
                    if( !argvn )  
					{
                        free( tmpb );
                        Free_Args( ct, argv );
                        return ( -1 );
                    }
                    if( ct )  
					{
                        memcpy( argvn,argv, ct * sizeof( char * ) );
                        free( argv );
                    }
                    argv = argvn;
                }

                if( !cch )  
				{
                    tmpb = ( char * )malloc( 1 );
                    if( !tmpb)  
					{
                        Free_Args( ct, argv );
                        return ( -1 );
                    }
                    tmpb[ 0 ] = '\0';
                }
                argv[ ct++ ] = tmpb;
                tmpb = NULL;
                cch = 0;
                mcch = 0;
            }
        } 
		else 
		{
            if( cch == mcch )  
			{
                mcch += 64;
                tmpbn = ( char * )malloc( mcch + 1 );
                if( !tmpbn )  
				{
                    free( tmpb );
                    Free_Args( ct, argv );
                    return ( -1 );
                }
                memset( tmpbn, 0, mcch + 1 );
                if( cch )  
				{
                    memcpy( tmpbn, tmpb, cch );
                    free( tmpb );
                }
                tmpb = tmpbn;
            }
            if( dq && ( cmdline[ i ] == '\\' ) && ( cmdline[ i + 1 ] == '\"' ) )  tmpb[ cch ] = cmdline[ i + 1 ];
            else tmpb[ cch ] = cmdline[ i ];
            cch++;
        }
    }

    if( pargv )  *pargv = argv;
    return (  ct ) ;
}









