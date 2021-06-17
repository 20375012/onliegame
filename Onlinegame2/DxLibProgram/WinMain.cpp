
/* ===============================================================================
|
|   WinMain
|
+ ============================================================================= */
#include "WINSOCK2.h"
#include "windows.h"
#include "time.h"
#include "Common.h"		// --- 全ファイルでの共通ヘッダー

int WINAPI WinMain(HINSTANCE hI,HINSTANCE hP,LPSTR lpC,int nC){

	int BGMLoopStartPosition = -1 ;
	int BGMLoopEndPosition = -1 ;
	VECTOR stagepos = VGet(0.0f,0.0f,0.0f) ;	//ステージのポジション
	int gamemode = eSceneBlank ;	// 全体的なゲームの管理


	ChangeWindowMode(TRUE) ;
	SetGraphMode(900,600,32) ;
	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	if(	SetServerClient()	== -1) return -1 ;		// set server with client
	if(	CameraPosition_Init()	== -1) return -1 ;	// set position_camera
	if(	DxLib_Init()		== -1) return -1 ;		// DXライブラリの初期化

	if(	Chara_player_init() == -1) return -1 ;
	if(	Chara_goblin_init() == -1) return -1 ;
	if(	Sound_Init()		== -1) return -1 ;


	// ------------------------------------------------------------------------ // 
	//							キャラ情報セット								//
	// ------------------------------------------------------------------------ //
	charainfo[0].pos = VGet(0.0f,500.0f,0.0f) ;		 // 草原
	charainfo[0].mode = STAND ;
	charainfo[0].direction = DOWN ;
	charainfo[0].charahitinfo.Height = PC_HEIGHT ;
	charainfo[0].charahitinfo.Width = PC_WIDTH ;
	charainfo[0].charahitinfo.CenterPosition = charainfo[0].pos ;
	charainfo[0].move = VGet(0.0f,0.0f,0.0f) ;
	charainfo[0].PolyCharaHitField[0] = VGet(0.0f,0.0f,0.0f) ;
	charainfo[0].PolyCharaHitField[1] = VGet(0.0f,0.0f,0.0f) ;
	charainfo[0].PolyCharaHitField[2] = VGet(0.0f,0.0f,0.0f) ;

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

	//	--------------------------------------------------------------------------------
	gbcharainfo[0].mode = STAND ;	
	gbcharainfo[0].pos = VGet(800.0f,400.0f,-400.0f) ;
	gbcharainfo[0].direction = DOWN ;
	gbcharainfo[0].charahitinfo.Height = PC_HEIGHT ;
	gbcharainfo[0].charahitinfo.Width = PC_WIDTH ;
	gbcharainfo[0].charahitinfo.CenterPosition = gbcharainfo[0].pos ;
	gbcharainfo[0].move = VGet(0.0f,0.0f,0.0f) ;

	//--------------------------------------------------------------------------------------------------------------
	gbcharainfo[0].model1 = MV1LoadModel("..\\Goblin\\Goblin.mv1") ;
	if(gbcharainfo[0].model1 == -1) return -1 ;
	gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
	gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
	// アニメーションして動いてもその場で動いてるような状態
	gbcharainfo[0].rootflm = MV1SearchFrame(gbcharainfo[0].model1,"root") ;
	MV1SetFrameUserLocalMatrix(gbcharainfo[0].model1,gbcharainfo[0].rootflm,MGetIdent()) ;
	gbcharainfo[0].playtime = 0.0f ;

	//--------------------------------------------------------------------------------------------------------------
	if(CHOICESTAGE == 1){
		stagedata = MV1LoadModel("..\\Stage\\Stage00.mv1") ;
		// シャドウマップハンドルの作成

		// ライトの方向を設定
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;

		// シャドウマップが想定するライトの方向もセット
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;

		// シャドウマップに描画する範囲を設定
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;

		stagedata_c = MV1LoadModel("..\\Stage\\Stage00_c.mv1") ;
		if(stagedata == -1) return -1 ;
	}
	else{
		stagedata = MV1LoadModel("..\\Stage\\Stage01.mv1") ;
		// シャドウマップハンドルの作成

		// ライトの方向を設定
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;
		// シャドウマップが想定するライトの方向もセット
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;
		// シャドウマップに描画する範囲を設定
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;
		stagedata_c = MV1LoadModel("..\\Stage\\Stage01_c.mv1") ;
		if(stagedata == -1) return -1 ;	
	}
	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo( stagedata, -1 ) ;
	SetDrawScreen(DX_SCREEN_BACK) ;
	int MeshNum ;
	// モデルに含まれるメッシュの数を取得する
    MeshNum = MV1GetMeshNum( stagedata ) ;
    // メッシュの数だけループ
    for(int i = 0 ; i < MeshNum ; i ++ ){
	// メッシュに含まれる頂点のローカル座標の最大座標値を描画
		Position = MV1GetMeshMaxPosition( stagedata, i ) ;
		DrawFormatString( 0, 112, GetColor( 255,255,255 ), "Max Position          x %f  y %f  z %f", Position.x, Position.y, Position.z ) ;

		// メッシュに含まれる頂点のローカル座標の最小座標値を描画
		Position = MV1GetMeshMinPosition( stagedata, i ) ;
		DrawFormatString( 0, 128, GetColor( 255,255,255 ), "Min Position          x %f  y %f  z %f", Position.x, Position.y, Position.z ) ;

	}
//--------------------------------------------------------------------------------------------------------------
//	PlaySoundMem( BGMSoundHandle, BGMLoopStartPosition >= 0 ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK) ;
//	PlaySoundMem( SESoundHandle, DX_PLAYTYPE_NORMAL) ;
//--------------------------------------------------------------------------------------------------------------

	// --- アプリケーションモード判定
	if ( strcmp( argv[ 1 ], "server" ) == 0 )		
	{
		app_mode	= SERVER_APP;	// --- サーバー
		game_mode	= GM_S_WAIT;	// --- 初期モード
	}
	else if ( strcmp( argv[ 1 ], "client" ) == 0 )	
	{
		app_mode	= CLIENT_APP;	// --- クライアント
		game_mode	= GM_C_FIND;	// --- 初期モード
	}
/*
	// --- アドレズサイズ格納
	addr_len = sizeof( recv_addr );	

	WSAStartup( version, &ws_data );	// --- ソケット作成( 本来ならエラー処理を入れるがソースが長くなるので省略 )

	sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( app_mode == SERVER_APP ){		// --- サーバー
			
		// --- ソケットデータの初期化
		addr.sin_family			= AF_INET;
		addr.sin_port			= htons( ( short )atoi( argv[ 3 ] ) );
		addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );

		WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
	}
	else if ( app_mode == CLIENT_APP ){	// --- クライアント
		// --- 接続先設定
		addr.sin_family			= AF_INET;								// --- インターネット
		addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- 接続ポート番号を数値にして設定
		addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- 接続IPアドレスを数値に変換
		WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
	}
*/
	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
					
		switch(gamemode){
			case eSceneBlank :
				gamemode = eScenePlay ;
				break ;
			case eScenePlay :

				//アニメーション
				if( Chara_player_Anim() == -1 ) return -1 ;
				if( Chara_goblin_Anim()== -1 ) return -1 ;

				
				if( app_mode == SERVER_APP ){
					Key_player();
				}else{
					Key_goblin();
				}
				ClearDrawScreen() ;
			
				//check 当たり判定...

				//床check
				if( Chara_player_fall() == -1) return -1 ;
				if( Chara_goblin_fall() == -1) return -1 ;

				// 移動処理
				for ( int i = 0 ; i < 3 ; i++ )
				{
					charainfo[i].pos.x += charainfo[i].move.x ;
					charainfo[i].pos.y += charainfo[i].move.y ;
					charainfo[i].pos.z += charainfo[i].move.z ;

					gbcharainfo[i].pos.x += gbcharainfo[i].move.x ;
					gbcharainfo[i].pos.y += gbcharainfo[i].move.y ;
					gbcharainfo[i].pos.z += gbcharainfo[i].move.z ;
				}

				//CAMERA
				if( CameraPosition() == -1) return -1 ;

				for ( int i = 0 ; i < 3 ; i++ )
				{
					MV1SetRotationXYZ(charainfo[i].model1,VGet(0.0f,1.57f * charainfo[0].direction,0.0f)) ;					// モデルの回転
					MV1SetPosition(charainfo[i].model1,charainfo[i].pos);													// モデルの移動(配置)
					MV1SetRotationXYZ(gbcharainfo[i].model1,VGet(0.0f,1.57f * gbcharainfo[0].direction,0.0f)) ;				// モデルの回転
					MV1SetPosition(gbcharainfo[i].model1,gbcharainfo[i].pos);												// モデルの移動(配置)
				}

				MV1SetPosition(stagedata,stagepos);						// 地面(配置)

				ShadowMap_DrawSetup( ShadowMapHandle ) ;				// シャドウマップへの描画の準備
				for ( int i = 0 ; i < 3 ; i++ )							// モデルの描画 - 影の描画
				{
					MV1DrawModel(charainfo[i].model1) ;
					MV1DrawModel(gbcharainfo[i].model1) ;
				}
				ShadowMap_DrawEnd() ;									// シャドウマップへの描画を終了
				SetUseShadowMap( 0, ShadowMapHandle ) ;					// 描画に使用するシャドウマップを設定

				MV1DrawModel(stagedata) ;								// 地面(配置)＆描画

				DrawTriangle3D(charainfo[0].PolyCharaHitField[0], charainfo[0].PolyCharaHitField[1], charainfo[0].PolyCharaHitField[2],GetColor( 255,0,0 ), TRUE ) ;
				DrawTriangle3D(gbcharainfo[0].PolyCharaHitField[0], gbcharainfo[0].PolyCharaHitField[1], gbcharainfo[0].PolyCharaHitField[2],GetColor( 255,255,0 ), TRUE ) ;
				
				for ( int i = 0 ; i < 3 ; i++ )							// モデルの描画
				{
					MV1DrawModel(charainfo[i].model1) ;
					MV1DrawModel(gbcharainfo[i].model1) ;
				}
				
				SetUseShadowMap( 0, -1 ) ;								// 描画に使用するシャドウマップの設定を解除
				ScreenFlip() ;
				break ;
			case eSceneClear :
				break ;
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

/* =======================================================================

+ ===================================================================== *
/*            hDC = GetDC( hWnd );
            hBitmap = CreateCompatibleBitmap( hDC, game_win.win_size.cx, game_win.win_size.cy );
            game_win.hScreenDC = CreateCompatibleDC( hDC );
            SelectObject( game_win.hScreenDC, hBitmap );
            ReleaseDC( hWnd, hDC );

			// --- ビットマップの読み込み
			hInst =	( HINSTANCE )GetWindowLong( hWnd, GWL_HINSTANCE );
			hBitmapSpr[ 0 ] = ( HBITMAP )LoadImage( NULL, SPR1_PASS, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			hBitmapSpr[ 1 ] = ( HBITMAP )LoadImage( NULL, SPR2_PASS, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			hBitmapSpr[ 2 ] = ( HBITMAP )LoadImage( NULL, SPR3_PASS, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
*
			// --- アプリケーションモード判定
			if ( strcmp( argv[ 1 ], "server" ) == 0 )		
			{
				app_mode	= SERVER_APP;	// --- サーバー
				game_mode	= GM_S_WAIT;	// --- 初期モード
				// --- コントロールの作成(チャット発言部分)
				edit_window = CreateWindow
				(
					"EDIT", "", 
					WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
					109,403,295,20, 
					hWnd,
					( HMENU )100,
					( HINSTANCE )GetWindowLong( hWnd, GWL_HINSTANCE ),
					NULL
				);

				// --- コントロールの作成(相手のメッセージ表示部分)
				static_window = CreateWindow
				(
					"STATIC", "", 
					WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
					109,56,295,20, 
					hWnd,
					( HMENU )100,
					( HINSTANCE )GetWindowLong( hWnd, GWL_HINSTANCE ),
					NULL
				);
			}
			else if ( strcmp( argv[ 1 ], "client" ) == 0 )	
			{
				app_mode	= CLIENT_APP;	// --- クライアント
				game_mode	= GM_C_FIND;	// --- 初期モード
				// --- コントロールの作成(チャット発言部分)
				edit_window = CreateWindow(
					"EDIT", "", 
					WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
					109,56,295,20, 
					hWnd,
					( HMENU )100,
					( HINSTANCE )GetWindowLong( hWnd, GWL_HINSTANCE ),
					NULL
				);

				// --- コントロールの作成(相手のメッセージ表示部分)
				static_window = CreateWindow(
					"STATIC", "", 
					WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
					109,403,295,20, 
					hWnd,
					( HMENU )100,
					( HINSTANCE )GetWindowLong( hWnd, GWL_HINSTANCE ),
					NULL
				);
			}

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

				// --- ソケットとポートを関連付ける
		//		bind( sock, ( struct sockaddr * )&addr, sizeof( addr ) );

				// --- ソケットを非ブロッキングにし、非同期通知を要求する
				WSAAsyncSelect( sock, hWnd, SM_EVENT, FD_READ | FD_WRITE );
			}
			else if ( app_mode == CLIENT_APP ){	// --- クライアント
				// --- 接続先設定
				addr.sin_family			= AF_INET;								// --- インターネット
				addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- 接続ポート番号を数値にして設定
				addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- 接続IPアドレスを数値に変換

				// --- ソケットを非ブロッキングにし、非同期通知を要求する
				WSAAsyncSelect( sock, hWnd, SM_EVENT, FD_READ | FD_WRITE );
			}

			// --- タイマーイベントセット( 約1秒間60フレーム 1000 / 60 = 16.666 )
*	    SetTimer( hWnd, 1, 17, NULL );    
			
			// --- 乱数をまわす
//			srand( ( unsigned )time( NULL ) );
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
			
			// --- デバッグ環境
			{
				
				wsprintf( str_debug, "文字列 : %d", GetWindowTextLength( edit_window ) );
			}
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
			// シャドウマップの削除
			DeleteShadowMap( ShadowMapHandle ) ;
			// ステージモデルの削除
			MV1DeleteModel( stagedata ) ;
			// キャラクターモデルの削除
			MV1DeleteModel( charainfo[0].model1 ) ;
			MV1DeleteModel( charainfo[1].model1 ) ;
			MV1DeleteModel( gbcharainfo[0].model1 ) ;

			DxLib_End() ;

			break;

        /* ---------------------------------
        |   デフォルトの処理
        + ------------------------------- 
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return( 0 );
}*/


