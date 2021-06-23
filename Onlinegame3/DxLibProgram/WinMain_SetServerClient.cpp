
/*------------------------------------------------
|
|			SetServerClient
|
+--------------------------------------------------*/
#include "Common.h"


int SetServerClient(void){

	// --- コマンドライン引数の形式に変換
	argc = Get_Args( GetCommandLine( ), &argv );
	// --- サーバー、クライアントを判定
	if ( argc > 2 ){
		if ( strcmp( argv[ 1 ], "server" ) == 0 ) {	// --- サーバー
			if ( argc != 4 ){
				MessageBox( NULL, "サーバー引数 : server 接続許可IPアドレス 解放ポート番号", "コマンドライン引数エラー", MB_OK );
				return( 0 );
			}
			wsprintf( test_str, "-+-+- サーバーアプリケーション -+-+-" );
		}
		else if ( strcmp( argv[ 1 ], "client" ) == 0 ){	// --- クライアント
			if ( argc != 4 ){
				MessageBox( NULL, "クライアント引数 : client サーバーIPアドレス ポート番号", "コマンドライン引数エラー", MB_OK );
				return( 0 );
			}
			wsprintf( test_str, "-+-+- クライアントアプリケーション -+-+-" );
		}
		else{
			MessageBox( NULL, "サーバーかクライアントを選んでください", "コマンドライン引数エラー", MB_OK );
			return( 0 );
		}
	}
	else{
		MessageBox( NULL, "サーバーかクライアントを選んでください", "コマンドライン引数エラー", MB_OK );
		return( 0 );
	}
	return 0;

}

int ChoseServerClient(void){
	/*--------------------------------------
	// --- アプリケーションモード判定
	+--------------------------------------*/
	if ( strcmp( argv[ 1 ], "server" ) == 0 )		
	{
		app_mode	= SERVER_APP;	// --- サーバー
		game_mode	= GM_S_WAIT;	// --- 初期モード
	}
	else if ( strcmp( argv[ 1 ], "client" ) == 0 )	
	{
		app_mode	= CLIENT_APP;	// --- クライアント
		game_mode	= GM_C_FIND;	// --- 初期モード
		free_mode	= NULL ;
	}
	gamemode = eSceneBlank ;
	return 0;

}

int Set_Sock(void){

		// --- アドレズサイズ格納
	    addr_len = sizeof( recv_addr );	

		// --- ソケット作成( 本来ならエラー処理を入れるがソースが長くなるので省略 )
		WSAStartup( version, &ws_data );
		sock = socket( AF_INET, SOCK_DGRAM, 0 );
		if ( app_mode == SERVER_APP ){		// --- サーバー
			
			// --- ソケットデータの初期化
			addr.sin_family			= AF_INET;
			addr.sin_port			= htons( ( short )atoi( argv[ 3 ] ) );
			addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );

			//// --- ソケットとポートを関連付ける
			//bind( sock, ( struct sockaddr * )&addr, sizeof( addr ) );

			// --- ソケットを非ブロッキングにし、非同期通知を要求する
			WSAAsyncSelect( sock, NULL , SM_EVENT, FD_READ | FD_WRITE );
		}
		else if ( app_mode == CLIENT_APP ){	// --- クライアント
			// --- 接続先設定
			addr.sin_family			= AF_INET;								// --- インターネット
			addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- 接続ポート番号を数値にして設定
			addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- 接続IPアドレスを数値に変換

			// --- ソケットを非ブロッキングにし、非同期通知を要求する
			WSAAsyncSelect( sock, NULL , SM_EVENT, FD_READ | FD_WRITE );
		}

	return 0 ;
}
