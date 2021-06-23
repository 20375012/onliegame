
/* ===============================================================================
|
|   WinMain
|
+ ============================================================================= */
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"		// --- 全ファイルでの共通ヘッダー

int WINAPI WinMain(HINSTANCE hI,HINSTANCE hP,LPSTR lpC,int nC){

	ChangeWindowMode(TRUE) ;
	SetGraphMode(900,600,32) ;
	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	if(	SetServerClient()		== -1)	return -1 ;		// set server with client
	if( ChoseServerClient()		== -1)  return -1 ;
	if(	DxLib_Init()			== -1)	return -1 ;		// DXライブラリの初期化
	if(	SetStage_Init()			== -1)	return -1 ;
	if(	Sound_Init()			== -1)	return -1 ;
	if(	Set_Sock()				== -1)	return -1 ;		//ソケット

	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
				
		switch(gamemode){
			case eSceneBlank :
				gamemode = eScenePlay ;
				break ;
			case eScenePlay :
				Game_Main_Function();
				break ;
			case eSceneClear :
				break ;
		}
		if( ProcessMessage() == SM_EVENT )
		{
			if ( app_mode == SERVER_APP )
			{
				recvfrom( sock, ( char * )&server_recv_work, sizeof( SERVER_RECV_DATA ), 0, ( struct sockaddr * )&recv_addr, &addr_len );
			}
			else
			{
				recvfrom( sock, ( char * )&client_recv_work, sizeof( CLIENT_RECV_DATA ), 0, ( struct sockaddr * )&recv_addr, &addr_len );
			}
			break;
		} 
	}


	DeleteShadowMap( ShadowMapHandle ) ;			// シャドウマップの削除
	MV1DeleteModel( stagedata ) ;					// ステージモデルの削除
	for ( int i = 0 ; i < 3 ; i++ )
	{
		MV1DeleteModel( charainfo[i].model1 ) ;			// キャラクターモデルの削除
		MV1DeleteModel( gbcharainfo[i].model1 ) ;		// キャラクターモデルの削除
	}

	DxLib_End() ;
	return 0 ;
}
/**/


/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : WinMain.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|       主にWinMainとウィンドウプロシージャーの２つの関数を記述
|       プログラムが走った直後に処理すべきコードの記述はここで行っても良い    
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ *

// --- 必要ファイルのインクルード
#include "WINSOCK2.h"
#include "windows.h"
#include "time.h"
#include "Common.h"      // --- ゲーム共通のヘッダーファイル

/* ===============================================================================
|
|   WinMain
|       メイン関数
+ --------------------------------------------------------------------------------
|   IN ---> HINSTANCE hInstance     --> ハンドル
|           HINSTANCE hPrevInstance --> 直前に起動した同一プログラムのハンドル
|           LPSTR lpCmdLine         --> コマンドライン引数
|           int nCmdShow            --> 起動時ウィンドウ状態
|   OUT --> int                     --> 終了値
+ ============================================================================= *
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASSEX  wcex;	// --- ウインドウクラス構造体
	HWND        hWnd;	// --- ウインドウハンドル
	MSG         msg;	// --- メッセージ構造体
	RECT        rect;
	int         ww, wh;
	int         cw, ch;

	if(	SetServerClient()		== -1)	return -1 ;		// set server with client

	// --- ウインドウクラス構造体を設定します。
	wcex.cbSize         = sizeof( WNDCLASSEX );
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = ( WNDPROC )WindowProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground  = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "ModelApp";
	wcex.hIconSm        = LoadIcon( NULL, IDI_APPLICATION );

	// --- ウインドウクラスを登録します。
	RegisterClassEx( &wcex );

	// --- ウインドウを作成します。
	hWnd = CreateWindow 
    ( 
        wcex.lpszClassName,		    // --- ウインドウクラス名
		test_str,					// --- キャプション文字列
		WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,		// --- ウインドウのスタイル
		CW_USEDEFAULT,				// --- 水平位置
		CW_USEDEFAULT,				// --- 垂直位置
		CLIENT_WIDTH,				// --- 幅
		CLIENT_HEIGHT,				// --- 高さ
		NULL,						// --- 親ウィンドウ
		NULL,                       // --- ウィンドウメニュー
		hInstance,                  // --- インスタンスハンドル
		NULL                        // --- WM_CREATE情報
    );

	// --- クライアント領域の外の幅を計算
	GetClientRect( hWnd, &rect );	// --- クライアント部分のサイズの取得
	cw = rect.right - rect.left;	// --- クライアント領域外の横幅を計算
	ch = rect.bottom - rect.top;	// --- クライアント領域外の縦幅を計算

	// --- ウインドウ全体の横幅の幅を計算
	GetWindowRect( hWnd, &rect );	// --- ウインドウ全体のサイズ取得
	ww = rect.right - rect.left;	// --- ウインドウ全体の幅の横幅を計算
	wh = rect.bottom - rect.top;	// --- ウインドウ全体の幅の縦幅を計算
	ww = ww - cw;					// --- クライアント領域以外に必要な幅
	wh = wh - ch;					// --- クライアント領域以外に必要な高さ

	// --- ウィンドウサイズの再計算
	ww = CLIENT_WIDTH + ww;			// --- 必要なウインドウの幅
	wh = CLIENT_HEIGHT + wh;		// --- 必要なウインドウの高さ

	// --- ウインドウサイズの再設定
	SetWindowPos( hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE );
	// --- ウィンドウを表示します。
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
	// --- メッセージループ

	ChangeWindowMode(TRUE) ;
	SetGraphMode(900,600,32) ;
	if( DxLib_Init()== -1 ) return -1;
	if( SetStage_Init() == -1 ) return -1;
//	if(	Sound_Init()			== -1)	return -1 ;

	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return( msg.wParam );
}

/* =======================================================================
|
|   WindowProc
|       ウィンドウプロシージャ
+ ------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|           UINT message        --> メッセージ
|           WPARAM wParam       --> メッセージ補足情報
|           LPARAM lParam       --> メッセージ補足情報
|   OUT --> LRESULT             --> 終了値
+ ===================================================================== *
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static HINSTANCE	hInst;
	HDC	                hDC;
	PAINTSTRUCT	        ps;
    RECT                rect;

	// --- メッセージの種類に応じて処理を分岐します。
	switch( message )
	{
        /* ---------------------------------
        |   WM_CREATEメッセージ
        + ------------------------------- *
		case WM_CREATE:
            // --- 裏画面の作成
            hDC = GetDC( hWnd );
//            hBitmap = CreateCompatibleBitmap( hDC, game_win.win_size.cx, game_win.win_size.cy );
//            game_win.hScreenDC = CreateCompatibleDC( hDC );
//            SelectObject( game_win.hScreenDC, hBitmap );

			ReleaseDC( hWnd, hDC );
			if(	Set_Sock()				== -1)	return -1 ;		//ソケット
			ChoseServerClient()	;	// set server with client
			// --- タイマーイベントセット( 約1秒間60フレーム 1000 / 60 = 16.666 )
		    SetTimer( hWnd, 1, 17, NULL );    
			
			// --- 乱数をまわす
			srand( ( unsigned )time( NULL ) );
			break;

        /* ---------------------------------
        |   WM_TIMERメッセージ
        + ------------------------------- *
		case WM_TIMER:
            Game_Main_Function( hWnd );
			InvalidateRect( hWnd, NULL, FALSE ) ;    // --- 再描画設定
			break;

        /* ---------------------------------
        |   WM_PAINTメッセージ
        + ------------------------------- *
		case WM_PAINT:
			hDC = BeginPaint( hWnd, &ps );
            GetClientRect( hWnd, &rect );
            BitBlt( hDC, 0, 0, rect.right, rect.bottom, game_win.hScreenDC, 0, 0, SRCCOPY );
			EndPaint ( hWnd, &ps );
			break;

        /* ---------------------------------
        |   SM_EVENTメッセージ
        + ------------------------------- *
		case SM_EVENT:
			{
				int sock_event_code;
				int sock_error_code;
				SOCKET active_sock;

				sock_event_code = WSAGETSELECTEVENT( lParam );
				sock_error_code = WSAGETSELECTERROR( lParam );
				
				active_sock = ( SOCKET )wParam;

				// --- イベントによって分岐
				switch ( sock_event_code )
				{
					/* ---------------------------------
					|   FD_READメッセージ
					+ ------------------------------- *
					case FD_READ:
						if ( app_mode == SERVER_APP )
						{
							recvfrom( sock, ( char * )&server_recv_work, sizeof( SERVER_RECV_DATA ), 0, ( struct sockaddr * )&recv_addr, &addr_len );
						}
						else
						{
							recvfrom( sock, ( char * )&client_recv_work, sizeof( CLIENT_RECV_DATA ), 0, ( struct sockaddr * )&recv_addr, &addr_len );
						}
						break;

					/* ---------------------------------
					|   FD_WHITEメッセージ
					+ ------------------------------- *
					case FD_WRITE:
						break;
				}
			} 
			break;	

        /* ---------------------------------
        |   WM_DESTROYメッセージ
        + ------------------------------- *
		case WM_DESTROY:
			KillTimer( hWnd, 1 );               // --- タイマーに使用しているリソース解放
			PostQuitMessage( 0 );
			Free_Args( argc, argv );
			break;

        /* ---------------------------------
        |   デフォルトの処理
        + ------------------------------- *
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return( 0 );
}
*/

