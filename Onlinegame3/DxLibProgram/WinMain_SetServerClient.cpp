
/*------------------------------------------------
|
|			SetServerClient
|
+--------------------------------------------------*/
#include "Common.h"


int SetServerClient(void){

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
	return 0;

}

int ChoseServerClient(void){
	/*--------------------------------------
	// --- �A�v���P�[�V�������[�h����
	+--------------------------------------*/
	if ( strcmp( argv[ 1 ], "server" ) == 0 )		
	{
		app_mode	= SERVER_APP;	// --- �T�[�o�[
		game_mode	= GM_S_WAIT;	// --- �������[�h
	}
	else if ( strcmp( argv[ 1 ], "client" ) == 0 )	
	{
		app_mode	= CLIENT_APP;	// --- �N���C�A���g
		game_mode	= GM_C_FIND;	// --- �������[�h
		free_mode	= NULL ;
	}
	gamemode = eSceneBlank ;
	return 0;

}

int Set_Sock(void){

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

			//// --- �\�P�b�g�ƃ|�[�g���֘A�t����
			//bind( sock, ( struct sockaddr * )&addr, sizeof( addr ) );

			// --- �\�P�b�g���u���b�L���O�ɂ��A�񓯊��ʒm��v������
			WSAAsyncSelect( sock, NULL , SM_EVENT, FD_READ | FD_WRITE );
		}
		else if ( app_mode == CLIENT_APP ){	// --- �N���C�A���g
			// --- �ڑ���ݒ�
			addr.sin_family			= AF_INET;								// --- �C���^�[�l�b�g
			addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- �ڑ��|�[�g�ԍ��𐔒l�ɂ��Đݒ�
			addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- �ڑ�IP�A�h���X�𐔒l�ɕϊ�

			// --- �\�P�b�g���u���b�L���O�ɂ��A�񓯊��ʒm��v������
			WSAAsyncSelect( sock, NULL , SM_EVENT, FD_READ | FD_WRITE );
		}

	return 0 ;
}
