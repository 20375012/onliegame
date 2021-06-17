
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
