//
///*-----------------------------------------------------------------
//|
//|
//+-------------------------------------------------------------------*/
//
//#define SM_EVENT			WM_USER + 1	// --- ソケットメッセージイベント
//#define MESSAGE_STR_MAX		256			// --- 最大発言数
//
//extern WORD					version;			// --- WinSockへの要求バージョン
//extern WSADATA				ws_data;			// --- WinSockのデータ
//extern SOCKET				sock;				// --- サーバー＆クライアントソケット
//extern struct sockaddr_in	addr;				// --- サーバー＆クライアントアドレス情報
//extern struct sockaddr_in	recv_addr;			// --- レシーブ用
//extern int					addr_len;			// --- レシーブ用アドレスサイズ
//
//// --- 通信データ情報
//typedef struct	// --- サーバー送信クライアント受信データ構造体
//{
//	int		mode;											// --- モード通知
//
//	int		chara_mode ;							// キャラの状態
//	VECTOR	chara_pos ;								// キャラの座標 x,y,z
//
//
//	char	message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
//
//} SERVER_SEND_DATA, CLIENT_RECV_DATA;
//
//typedef struct	// --- サーバー受信クライアント送信データ構造体
//{
//	int		mode;											// --- モード通知			
//
//	int		chara_mode ;							// キャラの状態
//	VECTOR	chara_pos ;								// キャラの座標 x,y,z
//
//	char	message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
//
//} SERVER_RECV_DATA, CLIENT_SEND_DATA;
//
////       プロトタイプ宣言
//int Game_Main_Function		( void );					// --- ゲームメイン関数
//// --- サーバー処理
//int Server_Game_Main			( void );				// --- サーバーゲーム処理
//	int GM_Server_Wait			( void );				// --- サーバークライアント待ち
//	int GM_Server_Start			( void );				// --- サーバー開始時処理
//	int GM_Server_Play			( void );				// --- サーバー対戦中時処理
//	int GM_Server_Play_1P		( void );				// --- サーバー1P処理
//	int GM_Server_Play_2P		( void );				// --- サーバー2P処理
//	int GM_Server_End			( void );				// --- サーバー終了処理
//	int ServerToClient_Send		( void );				// --- サーバーからクライアントへ送信
//	int Server_Disp_Show		( void );				// --- サーバー描画処理
//
//// --- クライアント処理
//int Client_Game_Main			( void );				// --- クライアントゲーム処理
//	int GM_Client_Find			( void );				// --- サーバークライアント待ち
//	int GM_Client_Start			( void );				// --- クライアント開始時処理
//	int GM_Client_Play			( void );				// --- クライアント対戦中時処理
//	int GM_Client_End			( void );				// --- クライアント終了処理
//	int ClientToServer_Send		( void );				// --- クライアントからサーバーへ送信
//	int Client_Disp_Show		( void );				// --- クライアント描画処理
//
//int Set(void);
//
//
