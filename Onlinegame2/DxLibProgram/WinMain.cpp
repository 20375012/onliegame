
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : WinMain.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|       ���WinMain�ƃE�B���h�E�v���V�[�W���[�̂Q�̊֐����L�q
|       �v���O����������������ɏ������ׂ��R�[�h�̋L�q�͂����ōs���Ă��ǂ�    
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- �K�v�t�@�C���̃C���N���[�h
#include "WINSOCK2.h"
#include "windows.h"
#include "time.h"
#include "Common.h"      // --- �Q�[�����ʂ̃w�b�_�[�t�@�C��

#define CHOICESTAGE 1


/* ===============================================================================
|
|   WinMain
|       ���C���֐�
+ --------------------------------------------------------------------------------
|   IN ---> HINSTANCE hInstance     --> �n���h��
|           HINSTANCE hPrevInstance --> ���O�ɋN����������v���O�����̃n���h��
|           LPSTR lpCmdLine         --> �R�}���h���C������
|           int nCmdShow            --> �N�����E�B���h�E���
|   OUT --> int                     --> �I���l
+ ============================================================================= */
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASSEX  wcex;	// --- �E�C���h�E�N���X�\����
	HWND        hWnd;	// --- �E�C���h�E�n���h��
	MSG         msg;	// --- ���b�Z�[�W�\����
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

	//�X�e�[�W�̃|�W�V����
	VECTOR stagepos = VGet(0.0f,0.0f,0.0f) ;

	// �V���h�E�}�b�v�n���h��
	int ShadowMapHandle ;

	VECTOR cpos,ctgt ;
	// �J�����|�W�V���� cpos:�J�����ʒu�@ctgt:�J���������_
	if(CHOICESTAGE == 1){
		cpos = VGet(0.0f,1400.0f,-1500.0f) ;
		ctgt = VGet(0.0f,500.0f,-400.0f) ;
	}
	else{
		cpos = VGet(500.0f,1501.0f,1800.0f) ;
		ctgt = VGet(500.0f,800.0f,2800.0f) ; // ���A
	}

	// �L�������
	SCharaInfo charainfo[5] ;

	// �X�e�[�W�R���W�������
	MV1_COLL_RESULT_POLY_DIM HitDim ;					// �L�����N�^�[�̎��͂ɂ���|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	int WallNum ;										// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int FloorNum ;										// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��

	int HitFlag ;										// �|���S���ɓ����������ǂ������L�����Ă����̂Ɏg���ϐ�( �O:�������Ă��Ȃ�  �P:�������� )
	MV1_COLL_RESULT_POLY *Poly ;						// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
	HITRESULT_LINE LineRes ;							// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����


	// �L�������q�b�g�������̃|���S���\���̍��W
	VECTOR PolyCharaHitField[3] ;

	int key ;

	VECTOR Position ;


	ChangeWindowMode(TRUE) ;

	SetGraphMode(900,600,32) ;

	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	// DX���C�u�����̏�����
	if(DxLib_Init() == -1) return -1 ;

	// �A�j���[�V�����̓ǂݍ���
	anim_nutral = MV1LoadModel("..\\Player\\Anim_Neutral.mv1") ;	// �����A�j��
	if(anim_nutral == -1) return -1 ;
	
	anim_run = MV1LoadModel("..\\Player\\Anim_Run.mv1") ;			// ����A�j��	
	if(anim_run == -1) return -1 ;

	anim_jumpin = MV1LoadModel("..\\Player\\Anim_Jump_In.mv1") ;		// �W�����v����n�߃A�j��
	if(anim_jumpin == -1) return -1 ;
	anim_jumploop = MV1LoadModel("..\\Player\\Anim_Jump_Loop.mv1") ;	// �W�����v���[�v�A�j��
	if(anim_jumploop == -1) return -1 ;
	anim_jumpout = MV1LoadModel("..\\Player\\Anim_Jump_Out.mv1") ;		// �W�����v���n�A�j��
	if(anim_jumpout == -1) return -1 ;
	anim_attack = MV1LoadModel("..\\Player\\Anim_Attack1.mv1") ;		// �A�^�b�N�A�j��
	if(anim_attack == -1) return -1 ;
	anim_attack2 = MV1LoadModel("..\\Player\\Anim_Attack2.mv1") ;		// �A�^�b�N�A�j��2
	if(anim_attack2 == -1) return -1 ;
	anim_attack3 = MV1LoadModel("..\\Player\\Anim_Attack3.mv1") ;		// �A�^�b�N�A�j��2
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
	//							�L�������Z�b�g								//
	// ------------------------------------------------------------------------ //
	if(CHOICESTAGE == 1){
		charainfo[0].pos = VGet(0.0f,500.0f,0.0f) ;		 // ����
	}
	else{
		charainfo[0].pos = VGet(500.0f,800.0f,2800.0f) ; // ���A
	}
	charainfo[0].mode = STAND ;
	charainfo[0].direction = DOWN ;
	charainfo[0].charahitinfo.Height = PC_HEIGHT ;
	charainfo[0].charahitinfo.Width = PC_WIDTH ;
	charainfo[0].charahitinfo.CenterPosition = charainfo[0].pos ;
	charainfo[0].move = VGet(0.0f,0.0f,0.0f) ;

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

	// ------------------------------------------------------------------------ // 
	//							�L�������Z�b�g								//
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
	//		�L�������f���Z�b�g	   //
	// --------------------------- // 
	charainfo[1].model1 = MV1DuplicateModel(charainfo[0].model1) ;
	if(charainfo[1].model1 == -1) return -1 ;
	charainfo[1].attachidx = MV1AttachAnim(charainfo[1].model1,0,anim_nutral) ;
	charainfo[1].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[1].model1,charainfo[1].attachidx) ;
	// �A�j���[�V�������ē����Ă����̏�œ����Ă�悤�ȏ��
	charainfo[1].rootflm = MV1SearchFrame(charainfo[1].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[1].model1,charainfo[1].rootflm,MGetIdent()) ;
	charainfo[1].playtime = 0.0f ;
//--------------------------------------------------------------------------------------------------------------
	charainfo[2].model1 = MV1LoadModel("..\\Goblin\\Goblin.mv1") ;
	if(charainfo[2].model1 == -1) return -1 ;
	charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_nutral) ;
	charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
	// �A�j���[�V�������ē����Ă����̏�œ����Ă�悤�ȏ��
	charainfo[2].rootflm = MV1SearchFrame(charainfo[2].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[2].model1,charainfo[2].rootflm,MGetIdent()) ;
	charainfo[2].playtime = 0.0f ;


	// --- �R�}���h���C�������̌`���ɕϊ�
	argc = Get_Args( GetCommandLine( ), &argv );
	// --- �T�[�o�[�A�N���C�A���g�𔻒�
	if ( argc > 2 ){
		if ( strcmp( argv[ 1 ], "server" ) == 0 ) {	// --- �T�[�o�[
			if ( argc != 4 ){
				MessageBox( NULL, "�T�[�o�[���� : server �ڑ�����IP�A�h���X ����|�[�g�ԍ�", "�R�}���h���C�������G���[", MB_OK );
				return( 0 );
			}
			wsprintf( test_str, "-+-+- �T�[�o�[�A�v���P�[�V���� -+-+-" );
		}
		else if ( strcmp( argv[ 1 ], "client" ) == 0 ){	// --- �N���C�A���g
			if ( argc != 4 ){
				MessageBox( NULL, "�N���C�A���g���� : client �T�[�o�[IP�A�h���X �|�[�g�ԍ�", "�R�}���h���C�������G���[", MB_OK );
				return( 0 );
			}
			wsprintf( test_str, "-+-+- �N���C�A���g�A�v���P�[�V���� -+-+-" );
		}
		else{
			MessageBox( NULL, "�T�[�o�[���N���C�A���g��I��ł�������", "�R�}���h���C�������G���[", MB_OK );
			return( 0 );
		}
	}
	else{
		MessageBox( NULL, "�T�[�o�[���N���C�A���g��I��ł�������", "�R�}���h���C�������G���[", MB_OK );
		return( 0 );
	}

	// --- �E�C���h�E�N���X�\���̂�ݒ肵�܂��B
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

	// --- �E�C���h�E�N���X��o�^���܂��B
	RegisterClassEx( &wcex );

	// --- �E�C���h�E���쐬���܂��B
	hWnd = CreateWindow 
    ( 
        wcex.lpszClassName,		    // --- �E�C���h�E�N���X��
		test_str,					// --- �L���v�V����������
		WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,		// --- �E�C���h�E�̃X�^�C��
		CW_USEDEFAULT,				// --- �����ʒu
		CW_USEDEFAULT,				// --- �����ʒu
		CLIENT_WIDTH,				// --- ��
		CLIENT_HEIGHT,				// --- ����
		NULL,						// --- �e�E�B���h�E
		NULL,                       // --- �E�B���h�E���j���[
		hInstance,                  // --- �C���X�^���X�n���h��
		NULL                        // --- WM_CREATE���
    );

	// --- �N���C�A���g�̈�̊O�̕����v�Z
	GetClientRect( hWnd, &rect );	// --- �N���C�A���g�����̃T�C�Y�̎擾
	cw = rect.right - rect.left;	// --- �N���C�A���g�̈�O�̉������v�Z
	ch = rect.bottom - rect.top;	// --- �N���C�A���g�̈�O�̏c�����v�Z

	// --- �E�C���h�E�S�̂̉����̕����v�Z
	GetWindowRect( hWnd, &rect );	// --- �E�C���h�E�S�̂̃T�C�Y�擾
	ww = rect.right - rect.left;	// --- �E�C���h�E�S�̂̕��̉������v�Z
	wh = rect.bottom - rect.top;	// --- �E�C���h�E�S�̂̕��̏c�����v�Z
	ww = ww - cw;					// --- �N���C�A���g�̈�ȊO�ɕK�v�ȕ�
	wh = wh - ch;					// --- �N���C�A���g�̈�ȊO�ɕK�v�ȍ���

	// --- �E�B���h�E�T�C�Y�̍Čv�Z
	ww = CLIENT_WIDTH + ww;			// --- �K�v�ȃE�C���h�E�̕�
	wh = CLIENT_HEIGHT + wh;		// --- �K�v�ȃE�C���h�E�̍���

	// --- �E�C���h�E�T�C�Y�̍Đݒ�
	SetWindowPos( hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE );

	// --- �E�B���h�E��\�����܂��B
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	// --- ���b�Z�[�W���[�v
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
|       �E�B���h�E�v���V�[�W��
+ ------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> �E�B���h�E�n���h��
|           UINT message        --> ���b�Z�[�W
|           WPARAM wParam       --> ���b�Z�[�W�⑫���
|           LPARAM lParam       --> ���b�Z�[�W�⑫���
|   OUT --> LRESULT             --> �I���l
+ ===================================================================== */
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	return( 0 );
}
































































































































































































































