
/* ===============================================================================
|
|   WinMain
|
+ ============================================================================= */
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"		// --- �S�t�@�C���ł̋��ʃw�b�_�[

int WINAPI WinMain(HINSTANCE hI,HINSTANCE hP,LPSTR lpC,int nC){

	ChangeWindowMode(TRUE) ;
	SetGraphMode(900,600,32) ;
	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	if(	SetServerClient()		== -1)	return -1 ;		// set server with client
	if( ChoseServerClient()		== -1)  return -1 ;
	if(	DxLib_Init()			== -1)	return -1 ;		// DX���C�u�����̏�����
	if(	SetStage_Init()			== -1)	return -1 ;
	if(	Sound_Init()			== -1)	return -1 ;
	if(	Set_Sock()				== -1)	return -1 ;		//�\�P�b�g

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
/**/


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
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ *

// --- �K�v�t�@�C���̃C���N���[�h
#include "WINSOCK2.h"
#include "windows.h"
#include "time.h"
#include "Common.h"      // --- �Q�[�����ʂ̃w�b�_�[�t�@�C��

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
+ ============================================================================= *
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASSEX  wcex;	// --- �E�C���h�E�N���X�\����
	HWND        hWnd;	// --- �E�C���h�E�n���h��
	MSG         msg;	// --- ���b�Z�[�W�\����
	RECT        rect;
	int         ww, wh;
	int         cw, ch;

	if(	SetServerClient()		== -1)	return -1 ;		// set server with client

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
|       �E�B���h�E�v���V�[�W��
+ ------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> �E�B���h�E�n���h��
|           UINT message        --> ���b�Z�[�W
|           WPARAM wParam       --> ���b�Z�[�W�⑫���
|           LPARAM lParam       --> ���b�Z�[�W�⑫���
|   OUT --> LRESULT             --> �I���l
+ ===================================================================== *
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static HINSTANCE	hInst;
	HDC	                hDC;
	PAINTSTRUCT	        ps;
    RECT                rect;

	// --- ���b�Z�[�W�̎�ނɉ����ď����𕪊򂵂܂��B
	switch( message )
	{
        /* ---------------------------------
        |   WM_CREATE���b�Z�[�W
        + ------------------------------- *
		case WM_CREATE:
            // --- ����ʂ̍쐬
            hDC = GetDC( hWnd );
//            hBitmap = CreateCompatibleBitmap( hDC, game_win.win_size.cx, game_win.win_size.cy );
//            game_win.hScreenDC = CreateCompatibleDC( hDC );
//            SelectObject( game_win.hScreenDC, hBitmap );

			ReleaseDC( hWnd, hDC );
			if(	Set_Sock()				== -1)	return -1 ;		//�\�P�b�g
			ChoseServerClient()	;	// set server with client
			// --- �^�C�}�[�C�x���g�Z�b�g( ��1�b��60�t���[�� 1000 / 60 = 16.666 )
		    SetTimer( hWnd, 1, 17, NULL );    
			
			// --- �������܂킷
			srand( ( unsigned )time( NULL ) );
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
			break;

        /* ---------------------------------
        |   �f�t�H���g�̏���
        + ------------------------------- *
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return( 0 );
}
*/

