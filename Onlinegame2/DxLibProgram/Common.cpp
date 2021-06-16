
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Common.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		�f�[�^��T�[�o�[&�N���C�A���g���ʂł̏����A�܂��͔ėp��������֐��̏���
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- �K�v�t�@�C���̃C���N���[�h
#include "WINSOCK2.h"
#include "windows.h"
#include <stdio.h>	// --- �R���\�[���p

#include "Common.h"      // --- �Q�[�����ʂ̃w�b�_�[�t�@�C��

/* -----------------------------------------------------------------------------------------
|
|
|       �f�[�^�錾
|
|
+ --------------------------------------------------------------------------------------- */
/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|       �e�[�u���f�[�^
+ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
// --- �I�Z���̔Ֆʃf�[�^
int osero_board_array[ OSERO_X_MAX * OSERO_Y_MAX ] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};


/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|       �ϐ��A�\���̃f�[�^
+ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
int				app_mode;						// --- �A�v���P�[�V�������[�h
int				game_mode;						// --- �Q�[�����[�h
int				free_mode;						// --- �t���[���[�h

int				play_mode[ PLYER_MAX ];			// --- �v���C���̃��[�h
int				chip_val[ PLYER_MAX ];			// --- �e�v���C���[�`�b�v��
int				turn_val;						// --- �^�[������ԍ�
char			message_str[ MESSAGE_STR_MAX ];	// --- ���b�Z�[�W������
int				non_cnt, black_cnt, white_cnt;	// --- �e�`�b�v�̃J�E���^

int				key_state[ KEY_MAX ];			// --- �L�[���
ConsoleWindow g_cWin;

POINT			mouse_point;					// --- �}�E�X�J�[�\���ʒu
HBITMAP			hBitmap;
HBITMAP			hBitmapSpr[ SPR_MAX ];
HWND	        edit_window; 
HWND		    static_window; 
GAMEWINDOW		game_win =
{
    NULL,
    NULL,
    { CLIENT_WIDTH, CLIENT_HEIGHT },
};


// --- �R�}���h���C������
int				argc;
char 			**argv;

// --- �ʐM�֘A�f�[�^
SERVER_SEND_DATA	server_sned_work;				// --- �T�[�o�[���M�f�[�^
CLIENT_RECV_DATA	client_recv_work;				// --- �N���C�A���g��M�f�[�^
SERVER_RECV_DATA	server_recv_work;				// --- �T�[�o�[��M�f�[�^
CLIENT_SEND_DATA	client_sned_work;				// --- �N���C�A���g���M�f�[�^


WORD                version = MAKEWORD( 2, 0 );     // --- WinSock�ւ̗v���o�[�W����
WSADATA             ws_data;                        // --- WinSock�̃f�[�^
SOCKET              sock;							// --- �T�[�o�[���N���C�A���g�\�P�b�g
struct sockaddr_in  addr;							// --- �T�[�o�[���N���C�A���g�A�h���X���
struct sockaddr_in  recv_addr;						// --- ���V�[�u�p
int					addr_len;						// --- ���V�[�u�p�A�h���X�T�C�Y

/* ===============================================================================
|
|   Game_Main_Function
|       �Q�[�����C���֐�
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> �E�B���h�E�n���h��
|           UINT message        --> ���b�Z�[�W
|           WPARAM wParam       --> ���b�Z�[�W�⑫���
|           LPARAM lParam       --> ���b�Z�[�W�⑫���
|   OUT --> LRESULT             --> �I���l
+ ============================================================================= */
int Game_Main_Function( HWND hWnd )
{
	// --- �L�[�擾
	key_state[ KEY_F1 ]			= GetKeyState( VK_F1 );
	key_state[ KEY_MOUSE_L ]	= GetKeyState( VK_LBUTTON );

	// --- �}�E�X�|�W�V�����擾
	GetCursorPos( &mouse_point );
	ScreenToClient( hWnd, &mouse_point );

    // --- �T�[�o�[���N���C�A���g�ŕ���
    switch( app_mode )
    {
        case SERVER_APP: Server_Game_Main( hWnd ); break;     // --- �T�[�o�Q�[�����C������
        case CLIENT_APP: Client_Game_Main( hWnd ); break;     // --- �N���C�A���g�Q�[�����C������
    }
    return( TRUE );
}

/* ===============================================================================
|
|   Finalize_Check
|       �I�����`�F�b�N
+ --------------------------------------------------------------------------------
|   IN ---> int		color1		--> �����̐F
|			int		color2		--> ����̐F
|   OUT --> int --> �I���l
+ ============================================================================= */
int Finalize_Check( void )
{
	int	i;

	// --- �Ֆʏ��S�ă`�F�b�N
	black_cnt = white_cnt = non_cnt = 0;
	for ( i = 0 ; i < OSERO_X_MAX * OSERO_Y_MAX ; i++ )
	{
		if ( osero_board_array[ i ] == OSERO_CHIP_NON )		{ non_cnt++; }		// --- �u����Ă��Ȃ������J�E���g
		if ( osero_board_array[ i ] == OSERO_CHIP_BLACK )	{ black_cnt++; }	// --- ���̐����J�E���g
		if ( osero_board_array[ i ] == OSERO_CHIP_WHITE )	{ white_cnt++; }	// --- ���̐����J�E���g
	}

	// --- �I���������`�F�b�N
	if ( black_cnt != 0 && white_cnt == 0 )	{ return( TRUE ); }	// --- ��������������
	if ( black_cnt == 0 && white_cnt != 0 )	{ return( TRUE ); }	// --- ��������������
	if ( non_cnt == 0 )						{ return( TRUE ); }	// --- �S�����܂��Ă�����

	return( FALSE );	// --- �p��
}

/* ===============================================================================
|
|   Pass_Check
|       �p�X��Ԃ��`�F�b�N
+ --------------------------------------------------------------------------------
|   IN ---> int		color1		--> �����̐F
|			int		color2		--> ����̐F
|   OUT --> int --> �I���l
+ ============================================================================= */
int Pass_Check( int color1, int color2 )
{
	int		i;
	int		flg = 0;
	POINT	point;
	
	// --- �Ֆʏ��S�ă`�F�b�N
	for ( i = 0 ; i < ( OSERO_X_MAX * OSERO_Y_MAX ) ; i++ )
	{
		// --- �󂢂Ă�����
		if ( osero_board_array[ i ] == OSERO_CHIP_NON )
		{
			// --- �|�C���g�i�[
			point.x = i % OSERO_X_MAX;
			point.y = i / OSERO_X_MAX;
		
			// --- ����̃`�b�v�����邩�𔻒�
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 0, 0, FALSE );	// --- ����������J�n
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 1, 0, FALSE );	// --- �E���������J�n
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 2, 0, FALSE );	// --- �����������J�n
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 3, 0, FALSE );	// --- �����������J�n
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 4, 0, FALSE );	// --- ������������J�n
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 5, 0, FALSE );	// --- ������������J�n
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 6, 0, FALSE );	// --- �E�����������J�n
			flg |= Chip_Chain_Reflexive_Func( point, osero_board_array, color1, color2, 7, 0, FALSE );	// --- �������������J�n

			// --- ��ł����Ă�����
			if ( flg == 1 )
			{
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

/* ===============================================================================
|
|   Chip_Set_Check
|       �`�b�v���u���邩�̔���
+ --------------------------------------------------------------------------------
|   IN ---> POINT	dest_point	--> �v�����ꂽ�|�C���g
|			int		color1		--> �����̐F
|			int		color2		--> ����̐F
|   OUT --> int --> �I���l
+ ============================================================================= */
int Chip_Set_Check( POINT dest_point, int color1, int color2 )
{
	int flg = 0;

	// --- �u�����Ɖ��肵���Ֆʃf�[�^
	int osero_board_virtual_array[ OSERO_X_MAX * OSERO_Y_MAX ];

	// --- ����̔Ֆʃf�[�^�̃R�s�[
	memcpy( osero_board_virtual_array, osero_board_array, sizeof( osero_board_array ) );

	// --- �Ֆʏ�̎w��ӏ����󂢂Ă�����
	if ( osero_board_array[ dest_point.x + ( dest_point.y * OSERO_X_MAX ) ] == OSERO_CHIP_NON )
	{
		osero_board_virtual_array[ dest_point.x + ( dest_point.y * OSERO_X_MAX ) ] = color1;	// --- �����̐F��u��

		// --- ����̃`�b�v�����邩�𔻒�
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 0, 0, TRUE );	// --- ����������J�n
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 1, 0, TRUE );	// --- �E���������J�n
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 2, 0, TRUE );	// --- �����������J�n
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 3, 0, TRUE );	// --- �����������J�n
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 4, 0, TRUE );	// --- ������������J�n
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 5, 0, TRUE );	// --- ������������J�n
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 6, 0, TRUE );	// --- �E�����������J�n
		flg |= Chip_Chain_Reflexive_Func( dest_point, osero_board_virtual_array, color1, color2, 7, 0, TRUE );	// --- �������������J�n
	}

	// --- 1�ł�����̃`�b�v����ꂽ��
	if ( flg == 1 )	
	{
		// --- ����̔Ֆʃf�[�^�̃R�s�[
		memcpy( osero_board_array, osero_board_virtual_array, sizeof( osero_board_array ) );
		return( 1 ); 
	}
	else
	{
		return( 0 ); 
	}	
}

/* ===============================================================================
|
|	Chip_Chain_Reflexive_Func
|       ����̃`�b�v����鏈��(�ċN�֐�)
+ --------------------------------------------------------------------------------
|   IN ---> POINT	point		--> �`�F�b�N�|�C���g
|			int		*array		--> �Ֆʂ̃|�C���^
|			int		color1		--> �����̐F
|			int		color2		--> ����̐F
|			int		direction	--> ��������
|			int		cnt			--> ���݂̏�����
|			int		option		--> �I�v�V����
|   OUT --> int --> �I���l
+ ============================================================================= */
int Chip_Chain_Reflexive_Func( POINT point, int *array, int color1, int color2, int direction, int cnt, int option )
{
	// --- �����Ŏw�肳�ꂽ�����ɃC���N�������g�A�f�N�������g
	if ( direction == 0 ) { point.y--; }			// --- �����
	if ( direction == 1 ) { point.x++; }			// --- �E����
	if ( direction == 2 ) { point.y++; }			// --- ������
	if ( direction == 3 ) { point.x--; }			// --- ������
	if ( direction == 4 ) { point.x--; point.y--; }	// --- �������
	if ( direction == 5 ) { point.x++; point.y--; }	// --- �E�����
	if ( direction == 6 ) { point.x++; point.y++; }	// --- �E������
	if ( direction == 7 ) { point.x--; point.y++; }	// --- ��������

	cnt++;	// --- �����񐔃C���N�������g

	// --- ���~�b�g�`�F�b�N
	if ( point.x < OSERO_X_MAX && point.y < OSERO_Y_MAX && point.x >= 0 && point.y >= 0 )
	{
		// --- �w�肳�ꂽ�|�C���g���`�F�b�N
		if ( array[ point.x + ( point.y * OSERO_X_MAX ) ] == color1 )		// --- �����̐F
		{
			// --- �ׂ���Ȃ������琬���R�[�h���s
			if ( cnt > 1 )	{ return( 1 ); }		// --- �����R�[�h
			else			{ return( 0 ); }	// --- �s�����R�[�h
		}
		else if ( array[ point.x + ( point.y * OSERO_X_MAX ) ] == color2 )	// --- ����̐F
		{
			// --- �����𑱍s���Ԃ�l�𔻒肵�ăe�[�u������������
			if ( Chip_Chain_Reflexive_Func( point, array, color1, color2, direction, cnt, option ) == TRUE )	// --- �����̐F�ɏ�������
			{
				if ( option == TRUE ) { array[ point.x + ( point.y * OSERO_X_MAX ) ] = color1; }
				return( 1 );	// --- �����R�[�h
			}
			else	// --- �����̐F�ɂ͂ł��Ȃ�����
			{
				return( 0 );	// --- �s�����R�[�h
			}
		}
		else	// --- ��������������
		{
			return( 0 );	// --- �s�����R�[�h
		}
	}
	else	// --- �͈͊O��������I��
	{
		return( 0 );	// --- �s�����R�[�h
	}
}

/* ===============================================================================
|
|   Chip_Val_Check
|       �w��̃`�b�v�𐔂���
+ --------------------------------------------------------------------------------
|   IN ---> int	color1	--> �w��̐F
|   OUT --> int --> �I���l
+ ============================================================================= */
int Chip_Val_Check( int color )
{
	int val = 0;
	int i;

	// --- ����̐��l��T��
	for ( i = 0 ; i < OSERO_X_MAX * OSERO_Y_MAX ; i++ )
	{
		if ( osero_board_array[ i ] == color ) { val++; }
	}
	return( val );	// --- ���ʂ�Ԃ�
}

/* =======================================================================
|
|   Free_Args
|       �������X�g����֐�
+ ------------------------------------------------------------------------
|   IN ---> int		argc        --> �R�}���h���C����
|           char	**argv      --> �R�}���h���C�����X�g
|   OUT --> LRESULT             --> �I���l
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
|       �������X�g�擾�֐�
+ ------------------------------------------------------------------------
|   IN ---> int		argc        --> �R�}���h���C����
|           char	**argv      --> �R�}���h���C�����X�g
|   OUT --> LRESULT             --> �I���l
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









