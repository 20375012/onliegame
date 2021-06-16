
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
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- 必要ファイルのインクルード
#include "WINSOCK2.h"
#include "windows.h"
#include "time.h"
#include "Common.h"      // --- ゲーム共通のヘッダーファイル

#define CHOICESTAGE 1


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
+ ============================================================================= */
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASSEX  wcex;	// --- ウインドウクラス構造体
	HWND        hWnd;	// --- ウインドウハンドル
	MSG         msg;	// --- メッセージ構造体
	RECT        rect;
	int         ww, wh;
	int         cw, ch;
	char		test_str[ 256 ];

	int anim_nutral,anim_run,anim_jumpin,anim_jumploop,anim_jumpout,anim_attack,anim_attack2,anim_attack3 ;
	int gbanim_nutral,gbanim_damage ;

	int stagedata ;
	int stagedata_c ; 

	char BGMO_FilePath[] = "BGM_stg0.ogg" ;
	char String[256] ;
	int BGMSoundHandle ;
	int BGMLoopStartPosition = -1 ;
	int BGMLoopEndPosition = -1 ;
	char SE_FilePath[] = "dmg_byAxe_00.wav" ;
	int SESoundHandle ;

	int gamemode = eSceneBlank ;

	//ステージのポジション
	VECTOR stagepos = VGet(0.0f,0.0f,0.0f) ;

	// シャドウマップハンドル
	int ShadowMapHandle ;

	VECTOR cpos,ctgt ;
	// カメラポジション cpos:カメラ位置　ctgt:カメラ注視点
	if(CHOICESTAGE == 1){
		cpos = VGet(0.0f,1400.0f,-1500.0f) ;
		ctgt = VGet(0.0f,500.0f,-400.0f) ;
	}
	else{
		cpos = VGet(500.0f,1501.0f,1800.0f) ;
		ctgt = VGet(500.0f,800.0f,2800.0f) ; // 洞窟
	}

	// キャラ情報
	SCharaInfo charainfo[5] ;

	// ステージコリジョン情報
	MV1_COLL_RESULT_POLY_DIM HitDim ;					// キャラクターの周囲にあるポリゴンを検出した結果が代入される当たり判定結果構造体
	int WallNum ;										// 壁ポリゴンと判断されたポリゴンの数
	int FloorNum ;										// 床ポリゴンと判断されたポリゴンの数
	MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列

	int HitFlag ;										// ポリゴンに当たったかどうかを記憶しておくのに使う変数( ０:当たっていない  １:当たった )
	MV1_COLL_RESULT_POLY *Poly ;						// ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
	HITRESULT_LINE LineRes ;							// 線分とポリゴンとの当たり判定の結果を代入する構造体


	// キャラがヒットした床のポリゴン表示の座標
	VECTOR PolyCharaHitField[3] ;

	int key ;

	VECTOR Position ;


	ChangeWindowMode(TRUE) ;

	SetGraphMode(900,600,32) ;

	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	// DXライブラリの初期化
	if(DxLib_Init() == -1) return -1 ;

	// アニメーションの読み込み
	anim_nutral = MV1LoadModel("..\\Player\\Anim_Neutral.mv1") ;	// 立ちアニメ
	if(anim_nutral == -1) return -1 ;
	
	anim_run = MV1LoadModel("..\\Player\\Anim_Run.mv1") ;			// 走りアニメ	
	if(anim_run == -1) return -1 ;

	anim_jumpin = MV1LoadModel("..\\Player\\Anim_Jump_In.mv1") ;		// ジャンプ入り始めアニメ
	if(anim_jumpin == -1) return -1 ;
	anim_jumploop = MV1LoadModel("..\\Player\\Anim_Jump_Loop.mv1") ;	// ジャンプループアニメ
	if(anim_jumploop == -1) return -1 ;
	anim_jumpout = MV1LoadModel("..\\Player\\Anim_Jump_Out.mv1") ;		// ジャンプ着地アニメ
	if(anim_jumpout == -1) return -1 ;
	anim_attack = MV1LoadModel("..\\Player\\Anim_Attack1.mv1") ;		// アタックアニメ
	if(anim_attack == -1) return -1 ;
	anim_attack2 = MV1LoadModel("..\\Player\\Anim_Attack2.mv1") ;		// アタックアニメ2
	if(anim_attack2 == -1) return -1 ;
	anim_attack3 = MV1LoadModel("..\\Player\\Anim_Attack3.mv1") ;		// アタックアニメ2
	if(anim_attack3 == -1) return -1 ;	
	gbanim_nutral = MV1LoadModel("..\\Goblin\\Anim_Neutral.mv1") ;		// 
	if(gbanim_nutral == -1) return -1 ;
	gbanim_damage = MV1LoadModel("..\\Goblin\\Anim_Damage.mv1") ;		//
	if(gbanim_damage == -1) return -1 ;


	sprintf_s( String, SOUND_DIRECTORY_PATH "BGM\\%s", BGMO_FilePath) ;
	BGMSoundHandle = LoadSoundMem( String ) ;
	if( BGMSoundHandle == -1 ) {
		return false ;
	}

	sprintf_s( String, SOUND_DIRECTORY_PATH "SE\\Player\\%s", SE_FilePath) ;
	SESoundHandle = LoadSoundMem( String ) ;
	if( SESoundHandle == -1 ) {
		return false ;
	}


	// ------------------------------------------------------------------------ // 
	//							キャラ情報セット								//
	// ------------------------------------------------------------------------ //
	if(CHOICESTAGE == 1){
		charainfo[0].pos = VGet(0.0f,500.0f,0.0f) ;		 // 草原
	}
	else{
		charainfo[0].pos = VGet(500.0f,800.0f,2800.0f) ; // 洞窟
	}
	charainfo[0].mode = STAND ;
	charainfo[0].direction = DOWN ;
	charainfo[0].charahitinfo.Height = PC_HEIGHT ;
	charainfo[0].charahitinfo.Width = PC_WIDTH ;
	charainfo[0].charahitinfo.CenterPosition = charainfo[0].pos ;
	charainfo[0].move = VGet(0.0f,0.0f,0.0f) ;

	// --------------------------- // 
	//		キャラモデルセット	   //
	// --------------------------- // 
	charainfo[0].model1 = MV1LoadModel("..\\Player\\PC.mv1") ;
	if(charainfo[0].model1 == -1) return -1 ;
	charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
	charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
	// アニメーションして動いてもその場で動いてるような状態
	charainfo[0].rootflm = MV1SearchFrame(charainfo[0].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[0].model1,charainfo[0].rootflm,MGetIdent()) ;
	charainfo[0].playtime = 0.0f ;

	// ------------------------------------------------------------------------ // 
	//							キャラ情報セット								//
	// ------------------------------------------------------------------------ // 
	charainfo[1].pos = VGet(1000.0f,0.0f,-400.0f) ;
	charainfo[1].direction = DOWN ;
	charainfo[1].charahitinfo.Height = PC_HEIGHT ;
	charainfo[1].charahitinfo.Width = PC_WIDTH ;
	charainfo[1].charahitinfo.CenterPosition = charainfo[1].pos ;
	charainfo[1].move = VGet(0.0f,0.0f,0.0f) ;
//	--------------------------------------------------------------------------------
	charainfo[2].mode = GSTAND ;
	charainfo[2].pos = VGet(800.0f,0.0f,-400.0f) ;
	charainfo[2].direction = DOWN ;
	charainfo[2].charahitinfo.Height = PC_HEIGHT ;
	charainfo[2].charahitinfo.Width = PC_WIDTH ;
	charainfo[2].charahitinfo.CenterPosition = charainfo[2].pos ;
	charainfo[2].move = VGet(0.0f,0.0f,0.0f) ;


	// --------------------------- // 
	//		キャラモデルセット	   //
	// --------------------------- // 
	charainfo[1].model1 = MV1DuplicateModel(charainfo[0].model1) ;
	if(charainfo[1].model1 == -1) return -1 ;
	charainfo[1].attachidx = MV1AttachAnim(charainfo[1].model1,0,anim_nutral) ;
	charainfo[1].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[1].model1,charainfo[1].attachidx) ;
	// アニメーションして動いてもその場で動いてるような状態
	charainfo[1].rootflm = MV1SearchFrame(charainfo[1].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[1].model1,charainfo[1].rootflm,MGetIdent()) ;
	charainfo[1].playtime = 0.0f ;
//--------------------------------------------------------------------------------------------------------------
	charainfo[2].model1 = MV1LoadModel("..\\Goblin\\Goblin.mv1") ;
	if(charainfo[2].model1 == -1) return -1 ;
	charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_nutral) ;
	charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
	// アニメーションして動いてもその場で動いてるような状態
	charainfo[2].rootflm = MV1SearchFrame(charainfo[2].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[2].model1,charainfo[2].rootflm,MGetIdent()) ;
	charainfo[2].playtime = 0.0f ;


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
+ ===================================================================== */
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	return( 0 );
}
































































































































































































































