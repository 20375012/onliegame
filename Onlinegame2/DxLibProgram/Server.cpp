
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Server.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		�T�[�o�[�A�v���P�[�V�����N�����̏���
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- �K�v�t�@�C���̃C���N���[�h
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"      // --- �Q�[�����ʂ̃w�b�_�[�t�@�C��

/* ===============================================================================
|
|   Server_Game_Main
|       �Q�[�����C���֐�
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> �E�B���h�E�n���h��
|   OUT --> int                 --> �I���l
+ ============================================================================= */
int Server_Game_Main( HWND hWnd )
{
	// --- �Q�[�����[�h����
	switch( game_mode )
	{
		case GM_S_WAIT:		GM_Server_Wait	( );		break;	// --- �N���C�A���g�҂�
		case GM_S_START:	GM_Server_Start	( );		break;	// --- �X�^�[�g
		case GM_S_PLAY:		GM_Server_Play	( );		break;	// --- �ΐ풆
		case GM_S_END:		GM_Server_End	( hWnd );	break;	// --- �I��
	}

	// --- �T�[�o�[�펞����	
//	ServerToClient_Send	( );		// --- ���M����
//	Server_Disp_Show	( hWnd );	// --- �`�揈��

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Wait
|       �T�[�o�[�N���C�A���g�҂�
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Server_Wait( void )
{	
	// --- �A�h���X���i�[����Ă�����
	if ( recv_addr.sin_addr.s_addr != 0 )
	{
		game_mode = GM_S_START;			// --- �Q�[���J�n
		server_sned_work.mode = 1;		// --- �Q�[���J�n�f�[�^�i�[
	}

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Start
|       �T�[�o�[�J�n������
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Server_Start( void )
{	
	// --- ��U��U����
	turn_val = 0;//rand( ) % 2;

	// --- �^�[������
	if ( turn_val == 0 )
	{
		play_mode[ PLYER_1P ] = GM_S_PLAY_TURN;		
		play_mode[ PLYER_2P ] = GM_S_PLAY_WAIT;	
		turn_val = -1;	
	}
	else if ( turn_val == 1 )
	{
		play_mode[ PLYER_1P ] = GM_S_PLAY_WAIT;		
		play_mode[ PLYER_2P ] = GM_S_PLAY_TURN;		
		turn_val = -1;	
	}

	// --- �ŏ���-1�ɂ��Ă���
	server_recv_work.mouse_point.x = server_recv_work.mouse_point.y = -1;

	game_mode = GM_S_PLAY;

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Play
|       �T�[�o�[�ΐ풆������
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Server_Play( void )
{
	GM_Server_Play_1P( );	// --- �T�[�o�[1P����
	GM_Server_Play_2P( );	// --- �T�[�o�[2P����

	// --- �^�[������
	if ( turn_val == 0 )
	{
		// --- �p�X����
		if ( Pass_Check( OSERO_CHIP_BLACK, OSERO_CHIP_WHITE ) == TRUE )
		{
			play_mode[ PLYER_1P ] = GM_S_PLAY_TURN;		
			play_mode[ PLYER_2P ] = GM_S_PLAY_WAIT;		
		}
		turn_val = -1;	
	}
	else if ( turn_val == 1 )
	{
		// --- �p�X����
		if ( Pass_Check( OSERO_CHIP_WHITE, OSERO_CHIP_BLACK ) == TRUE )
		{
			play_mode[ PLYER_1P ] = GM_S_PLAY_WAIT;		
			play_mode[ PLYER_2P ] = GM_S_PLAY_TURN;	
		}
		turn_val = -1;	
	}

	// --- �`�b�v���擾
	chip_val[ PLYER_1P ] = Chip_Val_Check( OSERO_CHIP_BLACK );
	chip_val[ PLYER_2P ] = Chip_Val_Check( OSERO_CHIP_WHITE );

	// --- ���b�Z�[�W�擾
	if ( ( key_state[ KEY_F1 ] < 0 ) && GetWindowTextLength( edit_window ) != 0 )
	{
		GetWindowText( edit_window, message_str, sizeof( message_str ) );	// --- ���b�Z�[�W�̎擾
		SetWindowText( edit_window, "" );									// --- ���b�Z�[�W�̃Z�b�g
	}

	// --- ���b�Z�[�W�\��
	SetWindowText( static_window, server_recv_work.message_str );	// --- ����̃��b�Z�[�W�̃Z�b�g

	// --- �I������
	if ( Finalize_Check( ) == TRUE )
	{
		server_sned_work.mode = 2;		// --- �Q�[���I���f�[�^�i�[
		game_mode = GM_S_END;
	}

	return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Play_1P
|       �T�[�o�[1P����
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Server_Play_1P( void )
{
	if ( play_mode[ PLYER_1P ] == GM_S_PLAY_TURN )
	{
		// --- �}�E�X�̍���������Ă�����
		if ( key_state[ KEY_MOUSE_L ] < 0 )
		{
			// --- �z����W���Z�o
			mouse_point.x = ( mouse_point.x - DISP_BOARD_X ) / ( SPR_CHIP_CUT_W + CHIP_ADJUST_X );
			mouse_point.y = ( mouse_point.y - DISP_BOARD_Y ) / ( SPR_CHIP_CUT_H + CHIP_ADJUST_Y );

			// --- ���~�b�g�`�F�b�N
			if ( mouse_point.x < OSERO_X_MAX && mouse_point.y < OSERO_Y_MAX && mouse_point.x >= 0 && mouse_point.y >= 0 )
			{
				if ( Chip_Set_Check( mouse_point, OSERO_CHIP_BLACK, OSERO_CHIP_WHITE ) == TRUE )
				{
					turn_val = 1;	// --- �^�[���`�F���W
				}
			}
		}
	}

	return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_Play_2P
|       �T�[�o�[2P����
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Server_Play_2P( void )
{
	if ( play_mode[ PLYER_2P ] == GM_S_PLAY_TURN )
	{
		if ( server_recv_work.mouse_point.x != -1 )
		{
			if ( Chip_Set_Check( server_recv_work.mouse_point, OSERO_CHIP_WHITE, OSERO_CHIP_BLACK ) == TRUE )
			{
				turn_val = 0;	// --- �^�[���`�F���W
			}
		}
	}
	
    return( TRUE );
}

/* ===============================================================================
|
|   GM_Server_End
|       �T�[�o�[�I������
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Server_End( HWND hWnd )
{
	char str[ 256 ];

	if ( black_cnt > white_cnt )		// --- ���̏���
	{
		wsprintf( str, " %d �� %d �ō��̏����ł��B\n OK�{�^���ŃA�v���P�[�V�������I�����܂��B", black_cnt, white_cnt );
	}
	else if ( black_cnt < white_cnt )	// --- ���̏���
	{
		wsprintf( str, " %d �� %d �Ŕ��̏����ł��B\n OK�{�^���ŃA�v���P�[�V�������I�����܂��B", black_cnt, white_cnt );
	}
	else								// --- ��������
	{
		wsprintf( str, " %d �� %d �ň��������ł��B\n OK�{�^���ŃA�v���P�[�V�������I�����܂��B", black_cnt, white_cnt );
	}

	game_mode++;
	MessageBox( hWnd, "�I��", "Result", MB_OK );	
    return( TRUE );
}

/* ===============================================================================
|
|   Server_Disp_Show
|       �T�[�o�[�`�揈��
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> �E�B���h�E�n���h��
|   OUT --> int                 --> �I���l
+ ============================================================================= *
int Server_Disp_Show( HWND hWnd )
{
	HDC	hDC;
	int i;
	//int val1, val2;

	hDC = CreateCompatibleDC( NULL );

	// --- �w�i��`��
    SelectObject( hDC, hBitmapSpr[ 0 ] );
	BitBlt( game_win.hScreenDC, 0, 0, game_win.win_size.cx, game_win.win_size.cy, hDC, 0, 0, SRCCOPY ) ;

	// --- �Տ�̃`�b�v��`��
	SelectObject( hDC, hBitmapSpr[ 1 ] );
	for ( i = 0 ; i < OSERO_X_MAX * OSERO_Y_MAX ; i++ )
	{	
		if ( osero_board_array[ i ] != OSERO_CHIP_NON )
		{
			TransparentBlt
			( 
				game_win.hScreenDC, 
				( ( i % OSERO_X_MAX ) * SPR_CHIP_CUT_W ) + DISP_BOARD_X + ( ( i % OSERO_X_MAX ) * CHIP_ADJUST_X ),
				( ( i / OSERO_Y_MAX ) * SPR_CHIP_CUT_H ) + DISP_BOARD_Y + ( ( i / OSERO_X_MAX ) * CHIP_ADJUST_Y ), 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				hDC, 
				( osero_board_array[ i ] - 1 ) * SPR_CHIP_CUT_W, 0, 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				RGB( 0, 255, 0 ) 
			);
		}
	}
/*
	// --- ���l���Z�b�g
	SelectObject( hDC, hBitmapSpr[ 2 ] );

	// --- PLAYER1�̃`�b�v���\��
	val1 = ( chip_val[ PLYER_1P ] / 10 ) % 10;	// --- �\�̈�
	val2 = ( chip_val[ PLYER_1P ] / 1 ) % 10;	// --- ��̈�

	TransparentBlt
	( 
		game_win.hScreenDC, 
		56, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val1 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);
	TransparentBlt
	( 
		game_win.hScreenDC, 
		88, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val2 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);

	// --- PLAYER2�̃`�b�v���\��
	val1 = ( chip_val[ PLYER_2P ] / 10 ) % 10;	// --- �\�̈�
	val2 = ( chip_val[ PLYER_2P ] / 1 ) % 10;	// --- ��̈�

	TransparentBlt
	( 
		game_win.hScreenDC, 
		520, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val1 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);
	TransparentBlt
	( 
		game_win.hScreenDC, 
		552, 216, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		hDC, 
		val2 * SPR_NUM_CUT_W, 0, SPR_NUM_CUT_W, SPR_NUM_CUT_H, 
		RGB( 0, 255, 0 ) 
	);
*//*
	DeleteDC( hDC ); 
	
    return( TRUE );
}

/* ===============================================================================
|
|   ServerToClient_Send
|       �T�[�o�[����N���C�A���g�֑��M
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= *
int ServerToClient_Send( void )
{
	// --- �f�[�^�̍쐬
	memcpy( server_sned_work.osero_board_array, osero_board_array, sizeof( osero_board_array ) );	// --- �Ֆʂ��R�s�[
	memcpy( server_sned_work.chip_val, chip_val, sizeof( chip_val ) );								// --- �`�b�v�����
	memcpy( server_sned_work.message_str, message_str, sizeof( message_str ) );						// --- ���b�Z�[�W������
	server_sned_work.non_cnt	= non_cnt;
	server_sned_work.black_cnt	= black_cnt;
	server_sned_work.white_cnt	= white_cnt;

	// --- �N���C�A���g�ɑ��M
	sendto( sock, ( const char * )&server_sned_work, sizeof( SERVER_SEND_DATA ), 0, ( struct sockaddr * )&recv_addr, sizeof( recv_addr ) );	
	
    return( TRUE );
}*/
