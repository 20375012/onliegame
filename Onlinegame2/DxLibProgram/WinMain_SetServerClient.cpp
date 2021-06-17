
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
