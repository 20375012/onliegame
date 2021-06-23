//
///*------------------------------------------------
//|
//|
//+--------------------------------------------------*/
//#include "WINSOCK2.h"
//#include "windows.h"
//#include "Sock_Server_Client.h"
//
//
//
//WORD                version = MAKEWORD( 2, 0 );     // --- WinSockへの要求バージョン
//WSADATA             ws_data;                        // --- WinSockのデータ
//SOCKET              sock;							// --- サーバー＆クライアントソケット
//struct sockaddr_in  addr;							// --- サーバー＆クライアントアドレス情報
//struct sockaddr_in  recv_addr;						// --- レシーブ用
//int					addr_len;						// --- レシーブ用アドレスサイズ
//
//
//
//int Set(void){
//
//		// --- アドレズサイズ格納
//	    addr_len = sizeof( recv_addr );	
//
//		// --- ソケット作成( 本来ならエラー処理を入れるがソースが長くなるので省略 )
//		WSAStartup( version, &ws_data );
//		sock = socket( AF_INET, SOCK_DGRAM, 0 );
//		if ( app_mode == SERVER_APP ){		// --- サーバー
//			
//			// --- ソケットデータの初期化
//			addr.sin_family			= AF_INET;
//			addr.sin_port			= htons( ( short )atoi( argv[ 3 ] ) );
//			addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );
//
//			// --- ソケットとポートを関連付ける
//			bind( sock, ( struct sockaddr * )&addr, sizeof( addr ) );
//
//			// --- ソケットを非ブロッキングにし、非同期通知を要求する
//			WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
//		}
//		else if ( app_mode == CLIENT_APP ){	// --- クライアント
//			// --- 接続先設定
//			addr.sin_family			= AF_INET;								// --- インターネット
//			addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- 接続ポート番号を数値にして設定
//			addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- 接続IPアドレスを数値に変換
//
//			// --- ソケットを非ブロッキングにし、非同期通知を要求する
//			WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
//		}
//
//	return 0 ;
//}