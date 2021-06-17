
/* ===============================================================================
|
|   WinMain
|
+ ============================================================================= */
#include "WINSOCK2.h"
#include "windows.h"
#include "time.h"
#include "Common.h"		// --- �S�t�@�C���ł̋��ʃw�b�_�[

int WINAPI WinMain(HINSTANCE hI,HINSTANCE hP,LPSTR lpC,int nC){

	int BGMLoopStartPosition = -1 ;
	int BGMLoopEndPosition = -1 ;
	VECTOR stagepos = VGet(0.0f,0.0f,0.0f) ;	//�X�e�[�W�̃|�W�V����
	int gamemode = eSceneBlank ;	// �S�̓I�ȃQ�[���̊Ǘ�


	ChangeWindowMode(TRUE) ;
	SetGraphMode(900,600,32) ;
	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	if(	SetServerClient()	== -1) return -1 ;		// set server with client
	if(	CameraPosition_Init()	== -1) return -1 ;	// set position_camera
	if(	DxLib_Init()		== -1) return -1 ;		// DX���C�u�����̏�����

	if(	Chara_player_init() == -1) return -1 ;
	if(	Chara_goblin_init() == -1) return -1 ;
	if(	Sound_Init()		== -1) return -1 ;


	// ------------------------------------------------------------------------ // 
	//							�L�������Z�b�g								//
	// ------------------------------------------------------------------------ //
	charainfo[0].pos = VGet(0.0f,500.0f,0.0f) ;		 // ����
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
	//		�L�������f���Z�b�g	   //
	// --------------------------- // 
	charainfo[0].model1 = MV1LoadModel("..\\Player\\PC.mv1") ;
	if(charainfo[0].model1 == -1) return -1 ;
	charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
	charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
	// �A�j���[�V�������ē����Ă����̏�œ����Ă�悤�ȏ��
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
	// �A�j���[�V�������ē����Ă����̏�œ����Ă�悤�ȏ��
	gbcharainfo[0].rootflm = MV1SearchFrame(gbcharainfo[0].model1,"root") ;
	MV1SetFrameUserLocalMatrix(gbcharainfo[0].model1,gbcharainfo[0].rootflm,MGetIdent()) ;
	gbcharainfo[0].playtime = 0.0f ;

	//--------------------------------------------------------------------------------------------------------------
	if(CHOICESTAGE == 1){
		stagedata = MV1LoadModel("..\\Stage\\Stage00.mv1") ;
		// �V���h�E�}�b�v�n���h���̍쐬

		// ���C�g�̕�����ݒ�
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;

		// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;

		// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;

		stagedata_c = MV1LoadModel("..\\Stage\\Stage00_c.mv1") ;
		if(stagedata == -1) return -1 ;
	}
	else{
		stagedata = MV1LoadModel("..\\Stage\\Stage01.mv1") ;
		// �V���h�E�}�b�v�n���h���̍쐬

		// ���C�g�̕�����ݒ�
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;
		// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;
		// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;
		stagedata_c = MV1LoadModel("..\\Stage\\Stage01_c.mv1") ;
		if(stagedata == -1) return -1 ;	
	}
	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo( stagedata, -1 ) ;
	SetDrawScreen(DX_SCREEN_BACK) ;
	int MeshNum ;
	// ���f���Ɋ܂܂�郁�b�V���̐����擾����
    MeshNum = MV1GetMeshNum( stagedata ) ;
    // ���b�V���̐��������[�v
    for(int i = 0 ; i < MeshNum ; i ++ ){
	// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�̍ő���W�l��`��
		Position = MV1GetMeshMaxPosition( stagedata, i ) ;
		DrawFormatString( 0, 112, GetColor( 255,255,255 ), "Max Position          x %f  y %f  z %f", Position.x, Position.y, Position.z ) ;

		// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�̍ŏ����W�l��`��
		Position = MV1GetMeshMinPosition( stagedata, i ) ;
		DrawFormatString( 0, 128, GetColor( 255,255,255 ), "Min Position          x %f  y %f  z %f", Position.x, Position.y, Position.z ) ;

	}
//--------------------------------------------------------------------------------------------------------------
//	PlaySoundMem( BGMSoundHandle, BGMLoopStartPosition >= 0 ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK) ;
//	PlaySoundMem( SESoundHandle, DX_PLAYTYPE_NORMAL) ;
//--------------------------------------------------------------------------------------------------------------

	// --- �A�v���P�[�V�������[�h����
	if ( strcmp( argv[ 1 ], "server" ) == 0 )		
	{
		app_mode	= SERVER_APP;	// --- �T�[�o�[
		game_mode	= GM_S_WAIT;	// --- �������[�h
	}
	else if ( strcmp( argv[ 1 ], "client" ) == 0 )	
	{
		app_mode	= CLIENT_APP;	// --- �N���C�A���g
		game_mode	= GM_C_FIND;	// --- �������[�h
	}
/*
	// --- �A�h���Y�T�C�Y�i�[
	addr_len = sizeof( recv_addr );	

	WSAStartup( version, &ws_data );	// --- �\�P�b�g�쐬( �{���Ȃ�G���[���������邪�\�[�X�������Ȃ�̂ŏȗ� )

	sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( app_mode == SERVER_APP ){		// --- �T�[�o�[
			
		// --- �\�P�b�g�f�[�^�̏�����
		addr.sin_family			= AF_INET;
		addr.sin_port			= htons( ( short )atoi( argv[ 3 ] ) );
		addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );

		WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
	}
	else if ( app_mode == CLIENT_APP ){	// --- �N���C�A���g
		// --- �ڑ���ݒ�
		addr.sin_family			= AF_INET;								// --- �C���^�[�l�b�g
		addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- �ڑ��|�[�g�ԍ��𐔒l�ɂ��Đݒ�
		addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- �ڑ�IP�A�h���X�𐔒l�ɕϊ�
		WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
	}
*/
	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
					
		switch(gamemode){
			case eSceneBlank :
				gamemode = eScenePlay ;
				break ;
			case eScenePlay :

				//�A�j���[�V����
				if( Chara_player_Anim() == -1 ) return -1 ;
				if( Chara_goblin_Anim()== -1 ) return -1 ;

				
				if( app_mode == SERVER_APP ){
					Key_player();
				}else{
					Key_goblin();
				}
				ClearDrawScreen() ;
			
				//check �����蔻��...

				//��check
				if( Chara_player_fall() == -1) return -1 ;
				if( Chara_goblin_fall() == -1) return -1 ;

				// �ړ�����
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
					MV1SetRotationXYZ(charainfo[i].model1,VGet(0.0f,1.57f * charainfo[0].direction,0.0f)) ;					// ���f���̉�]
					MV1SetPosition(charainfo[i].model1,charainfo[i].pos);													// ���f���̈ړ�(�z�u)
					MV1SetRotationXYZ(gbcharainfo[i].model1,VGet(0.0f,1.57f * gbcharainfo[0].direction,0.0f)) ;				// ���f���̉�]
					MV1SetPosition(gbcharainfo[i].model1,gbcharainfo[i].pos);												// ���f���̈ړ�(�z�u)
				}

				MV1SetPosition(stagedata,stagepos);						// �n��(�z�u)

				ShadowMap_DrawSetup( ShadowMapHandle ) ;				// �V���h�E�}�b�v�ւ̕`��̏���
				for ( int i = 0 ; i < 3 ; i++ )							// ���f���̕`�� - �e�̕`��
				{
					MV1DrawModel(charainfo[i].model1) ;
					MV1DrawModel(gbcharainfo[i].model1) ;
				}
				ShadowMap_DrawEnd() ;									// �V���h�E�}�b�v�ւ̕`����I��
				SetUseShadowMap( 0, ShadowMapHandle ) ;					// �`��Ɏg�p����V���h�E�}�b�v��ݒ�

				MV1DrawModel(stagedata) ;								// �n��(�z�u)���`��

				DrawTriangle3D(charainfo[0].PolyCharaHitField[0], charainfo[0].PolyCharaHitField[1], charainfo[0].PolyCharaHitField[2],GetColor( 255,0,0 ), TRUE ) ;
				DrawTriangle3D(gbcharainfo[0].PolyCharaHitField[0], gbcharainfo[0].PolyCharaHitField[1], gbcharainfo[0].PolyCharaHitField[2],GetColor( 255,255,0 ), TRUE ) ;
				
				for ( int i = 0 ; i < 3 ; i++ )							// ���f���̕`��
				{
					MV1DrawModel(charainfo[i].model1) ;
					MV1DrawModel(gbcharainfo[i].model1) ;
				}
				
				SetUseShadowMap( 0, -1 ) ;								// �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
				ScreenFlip() ;
				break ;
			case eSceneClear :
				break ;
		}

	}


	DeleteShadowMap( ShadowMapHandle ) ;			// �V���h�E�}�b�v�̍폜
	MV1DeleteModel( stagedata ) ;					// �X�e�[�W���f���̍폜
	for ( int i = 0 ; i < 3 ; i++ )
	{
		MV1DeleteModel( charainfo[i].model1 ) ;			// �L�����N�^�[���f���̍폜
		MV1DeleteModel( gbcharainfo[i].model1 ) ;		// �L�����N�^�[���f���̍폜
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

			// --- �r�b�g�}�b�v�̓ǂݍ���
			hInst =	( HINSTANCE )GetWindowLong( hWnd, GWL_HINSTANCE );
			hBitmapSpr[ 0 ] = ( HBITMAP )LoadImage( NULL, SPR1_PASS, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			hBitmapSpr[ 1 ] = ( HBITMAP )LoadImage( NULL, SPR2_PASS, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			hBitmapSpr[ 2 ] = ( HBITMAP )LoadImage( NULL, SPR3_PASS, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
*
			// --- �A�v���P�[�V�������[�h����
			if ( strcmp( argv[ 1 ], "server" ) == 0 )		
			{
				app_mode	= SERVER_APP;	// --- �T�[�o�[
				game_mode	= GM_S_WAIT;	// --- �������[�h
				// --- �R���g���[���̍쐬(�`���b�g��������)
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

				// --- �R���g���[���̍쐬(����̃��b�Z�[�W�\������)
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
				app_mode	= CLIENT_APP;	// --- �N���C�A���g
				game_mode	= GM_C_FIND;	// --- �������[�h
				// --- �R���g���[���̍쐬(�`���b�g��������)
				edit_window = CreateWindow(
					"EDIT", "", 
					WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
					109,56,295,20, 
					hWnd,
					( HMENU )100,
					( HINSTANCE )GetWindowLong( hWnd, GWL_HINSTANCE ),
					NULL
				);

				// --- �R���g���[���̍쐬(����̃��b�Z�[�W�\������)
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

			// --- �A�h���Y�T�C�Y�i�[
	        addr_len = sizeof( recv_addr );	

			// --- �\�P�b�g�쐬( �{���Ȃ�G���[���������邪�\�[�X�������Ȃ�̂ŏȗ� )
			WSAStartup( version, &ws_data );
			sock = socket( AF_INET, SOCK_DGRAM, 0 );
			if ( app_mode == SERVER_APP ){		// --- �T�[�o�[
			
				// --- �\�P�b�g�f�[�^�̏�����
				addr.sin_family			= AF_INET;
				addr.sin_port			= htons( ( short )atoi( argv[ 3 ] ) );
				addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );

				// --- �\�P�b�g�ƃ|�[�g���֘A�t����
		//		bind( sock, ( struct sockaddr * )&addr, sizeof( addr ) );

				// --- �\�P�b�g���u���b�L���O�ɂ��A�񓯊��ʒm��v������
				WSAAsyncSelect( sock, hWnd, SM_EVENT, FD_READ | FD_WRITE );
			}
			else if ( app_mode == CLIENT_APP ){	// --- �N���C�A���g
				// --- �ڑ���ݒ�
				addr.sin_family			= AF_INET;								// --- �C���^�[�l�b�g
				addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- �ڑ��|�[�g�ԍ��𐔒l�ɂ��Đݒ�
				addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- �ڑ�IP�A�h���X�𐔒l�ɕϊ�

				// --- �\�P�b�g���u���b�L���O�ɂ��A�񓯊��ʒm��v������
				WSAAsyncSelect( sock, hWnd, SM_EVENT, FD_READ | FD_WRITE );
			}

			// --- �^�C�}�[�C�x���g�Z�b�g( ��1�b��60�t���[�� 1000 / 60 = 16.666 )
*	    SetTimer( hWnd, 1, 17, NULL );    
			
			// --- �������܂킷
//			srand( ( unsigned )time( NULL ) );
			break;

        /* ---------------------------------
        |   WM_TIMER���b�Z�[�W
        + ------------------------------- *
		case WM_TIMER:
            Game_Main_Function( hWnd );
			InvalidateRect( hWnd, NULL, FALSE ) ;    // --- �ĕ`��ݒ�
			break;

        /* ---------------------------------
        |   WM_PAINT���b�Z�[�W
        + ------------------------------- *
		case WM_PAINT:
			hDC = BeginPaint( hWnd, &ps );
            GetClientRect( hWnd, &rect );
            BitBlt( hDC, 0, 0, rect.right, rect.bottom, game_win.hScreenDC, 0, 0, SRCCOPY );
			
			// --- �f�o�b�O��
			{
				
				wsprintf( str_debug, "������ : %d", GetWindowTextLength( edit_window ) );
			}
			EndPaint ( hWnd, &ps );
			break;

        /* ---------------------------------
        |   SM_EVENT���b�Z�[�W
        + ------------------------------- *
		case SM_EVENT:
			{
				int sock_event_code;
				int sock_error_code;
				SOCKET active_sock;

				sock_event_code = WSAGETSELECTEVENT( lParam );
				sock_error_code = WSAGETSELECTERROR( lParam );
				
				active_sock = ( SOCKET )wParam;

				// --- �C�x���g�ɂ���ĕ���
				switch ( sock_event_code )
				{
					/* ---------------------------------
					|   FD_READ���b�Z�[�W
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
					|   FD_WHITE���b�Z�[�W
					+ ------------------------------- *
					case FD_WRITE:
						break;
				}
			} 
			break;	

        /* ---------------------------------
        |   WM_DESTROY���b�Z�[�W
        + ------------------------------- *
		case WM_DESTROY:
			KillTimer( hWnd, 1 );               // --- �^�C�}�[�Ɏg�p���Ă��郊�\�[�X���
			PostQuitMessage( 0 );
			Free_Args( argc, argv );
			// �V���h�E�}�b�v�̍폜
			DeleteShadowMap( ShadowMapHandle ) ;
			// �X�e�[�W���f���̍폜
			MV1DeleteModel( stagedata ) ;
			// �L�����N�^�[���f���̍폜
			MV1DeleteModel( charainfo[0].model1 ) ;
			MV1DeleteModel( charainfo[1].model1 ) ;
			MV1DeleteModel( gbcharainfo[0].model1 ) ;

			DxLib_End() ;

			break;

        /* ---------------------------------
        |   �f�t�H���g�̏���
        + ------------------------------- 
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return( 0 );
}*/


