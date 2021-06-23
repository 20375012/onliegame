
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- �K�v�t�@�C���̃C���N���[�h

#include <windows.h>
#include <stdio.h>	// --- �R���\�[���p
#include "Common.h"
    
HWND	        edit_window; 

/*-----------------------------------------------------------------------------------------------------------*/
int				app_mode;						// --- �A�v���P�[�V�������[�h
int				game_mode;						// --- �Q�[�����[�h
int				free_mode;						// --- �t���[���[�h

int				play_mode[ PLYER_MAX ];			// --- �v���C���̃��[�h
char			message_str[ MESSAGE_STR_MAX ];	// --- ���b�Z�[�W������
int				key_state[ KEY_MAX ];			// --- �L�[���
int				argc;
char 			**argv;

ConsoleWindow	g_cWin;
POINT			mouse_point;					// --- �}�E�X�J�[�\���ʒu
GAMEWINDOW		game_win =
{
    NULL,
    NULL,
    { CLIENT_WIDTH, CLIENT_HEIGHT },
};
// --- �ʐM�֘A�f�[�^
SERVER_SEND_DATA	server_send_work;				// --- �T�[�o�[���M�f�[�^
CLIENT_RECV_DATA	client_recv_work;				// --- �N���C�A���g��M�f�[�^
SERVER_RECV_DATA	server_recv_work;				// --- �T�[�o�[��M�f�[�^
CLIENT_SEND_DATA	client_send_work;				// --- �N���C�A���g���M�f�[�^


WORD                version = MAKEWORD( 2, 0 );     // --- WinSock�ւ̗v���o�[�W����
WSADATA             ws_data;                        // --- WinSock�̃f�[�^
SOCKET              sock;							// --- �T�[�o�[���N���C�A���g�\�P�b�g
struct sockaddr_in  addr;							// --- �T�[�o�[���N���C�A���g�A�h���X���
struct sockaddr_in  recv_addr;						// --- ���V�[�u�p
int					addr_len;						// --- ���V�[�u�p�A�h���X�T�C�Y

/*----------------------------------------------
|
|
-------------------------------------------------------------*/
char test_str[ 256 ];
char String[256] ;

SCharaInfo charainfo[3] ;	// �L�������
int anim_nutral,anim_run,anim_jumpin,anim_jumploop,anim_jumpout,anim_attack,anim_attack2,anim_attack3 ;

SCharaInfo gbcharainfo[3] ;	// �L�������
int gbanim_nutral,gbanim_damage,gbanim_run ;

int gamemode ;				// �S�̓I�ȃQ�[���̊Ǘ�
int stagedata ;
int stagedata_c ; 

int key ;

int BGMSoundHandle ;
int SESoundHandle ;
int ShadowMapHandle ;	// �V���h�E�}�b�v�n���h��

VECTOR stagepos ;	
VECTOR Position ;

MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
MV1_COLL_RESULT_POLY *Poly ;						// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
HITRESULT_LINE LineRes ;							// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����

/* ===============================================================================
|
|   Game_Main_Function
|
+ ============================================================================= */
int Game_Main_Function( void )
//int Game_Main_Function( HWND hWnd )
{
    switch( app_mode )
    {
        case SERVER_APP: Server_Game_Main( ); break;     // --- �T�[�o�Q�[�����C������
        case CLIENT_APP: Client_Game_Main( ); break;     // --- �N���C�A���g�Q�[�����C������
//        case SERVER_APP: Server_Game_Main( hWnd ); break;     // --- �T�[�o�Q�[�����C������
//        case CLIENT_APP: Client_Game_Main( hWnd ); break;     // --- �N���C�A���g�Q�[�����C������
    }
    return( TRUE );
}

/* ===============================================================================
|
|   Game_END
|
+ ============================================================================= */
int Finalize_Check( void )
{
	int i = 0 ;
	if( i != 0 ){
		return TRUE ;
	}
	return( FALSE );	// --- �p��
}
/* =======================================================================
|
|   Free_Args
|
+ ===================================================================== */
void Free_Args( int argc, char **argv )
{
    int i;

    for( i = 0 ; i < argc ; i++ ) free( argv[ i ] );

    free( argv );
}

/* =======================================================================
|
|   Get_Args
|
+ ===================================================================== */
int Get_Args( const char *cmdline, char ***pargv ) 
{
    int len, i, ct = 0, mct = 0, cch = 0, mcch = 0, dq = 0;
    
	char *tmpb = NULL, *tmpbn, **argv, **argvn;

    len = strlen( cmdline );
    for( i = 0 ; i <= len ; i++ )  
	{
        if( cmdline[ i ] == '\"' )  
		{
            if( dq )
			{
                if( cmdline[ i + 1 ] == ' ' )  dq = 0;
            } 
			else
			{ 
				dq = 1;
			}
        } 
		else if( ( ( !dq ) && ( cmdline[ i ] == ' ' ) ) || ( i == len ) )  
		{
            if( cch||dq||( i ? ( cmdline[ i - 1 ] == '\"' ) : 0 ) )  
			{
                if( ct == mct )  
				{
                    mct += 16;
                    argvn = ( char ** )calloc( mct, sizeof( char * ) );
                    if( !argvn )  
					{
                        free( tmpb );
                        Free_Args( ct, argv );
                        return ( -1 );
                    }
                    if( ct )  
					{
                        memcpy( argvn,argv, ct * sizeof( char * ) );
                        free( argv );
                    }
                    argv = argvn;
                }

                if( !cch )  
				{
                    tmpb = ( char * )malloc( 1 );
                    if( !tmpb)  
					{
                        Free_Args( ct, argv );
                        return ( -1 );
                    }
                    tmpb[ 0 ] = '\0';
                }
                argv[ ct++ ] = tmpb;
                tmpb = NULL;
                cch = 0;
                mcch = 0;
            }
        } 
		else 
		{
            if( cch == mcch )  
			{
                mcch += 64;
                tmpbn = ( char * )malloc( mcch + 1 );
                if( !tmpbn )  
				{
                    free( tmpb );
                    Free_Args( ct, argv );
                    return ( -1 );
                }
                memset( tmpbn, 0, mcch + 1 );
                if( cch )  
				{
                    memcpy( tmpbn, tmpb, cch );
                    free( tmpb );
                }
                tmpb = tmpbn;
            }
            if( dq && ( cmdline[ i ] == '\\' ) && ( cmdline[ i + 1 ] == '\"' ) )  tmpb[ cch ] = cmdline[ i + 1 ];
            else tmpb[ cch ] = cmdline[ i ];
            cch++;
        }
    }

    if( pargv )  *pargv = argv;
    return (  ct ) ;
}

/*------------------------------------------------
|
|
+-------------------------------------------------*/
int Sound_Init( void ){

	int BGMLoopStartPosition = -1 ;
	int BGMLoopEndPosition = -1 ;
	char BGMO_FilePath[13] = "BGM_stg0.ogg" ;
	char SE_FilePath[17] = "dmg_byAxe_00.wav" ;

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


//	PlaySoundMem( BGMSoundHandle, BGMLoopStartPosition >= 0 ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK) ;
//	PlaySoundMem( SESoundHandle, DX_PLAYTYPE_NORMAL) ;

	return 0;
}









