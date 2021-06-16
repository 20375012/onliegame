
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Client.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		�N���C�A���g�A�v���P�[�V�����N�����̏���
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- �K�v�t�@�C���̃C���N���[�h
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"      // --- �Q�[�����ʂ̃w�b�_�[�t�@�C��

/* ===============================================================================
|
|   Client_Game_Main
|       �Q�[�����C���֐�
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> �E�B���h�E�n���h��
|   OUT --> int                 --> �I���l
+ ============================================================================= */
int Client_Game_Main( HWND hWnd )
{
	// --- �Q�[�����[�h����
	switch( game_mode )
	{
		case GM_C_FIND:		GM_Client_Find	( );	break;	// --- �T�[�o�[�T��
		case GM_C_START:	GM_Client_Start	( );	break;	// --- �X�^�[�g
		case GM_C_PLAY:		GM_Client_Play	( );	break;	// --- �ΐ풆
		case GM_C_END:		GM_Client_End	( );	break;	// --- �I��
	}

	// --- �N���C�A���g�펞����	
//	ClientToServer_Send	( );		// --- ���M����
//	Client_Disp_Show	( hWnd );	// --- �`�揈��

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Client_Find
|       �T�[�o�[�T��
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Client_Find( void )
{	
	// --- �T�[�o�[�ɑ��M��A�J�n��҂�
	if ( free_mode == 0 )
	{
		client_sned_work.mode = 1;	// --- �T�[�o�[�ɒʒB
		free_mode++;
	}
	else
	{
		// --- �T�[�o�[����J�n�v�����������Ă�����
		if ( client_recv_work.mode == 1 )
		{
			free_mode = 0;
			game_mode = GM_C_START;
		}
	}

    return( TRUE );
}

/* ===============================================================================
|
|   GM_Client_Start
|       �N���C�A���g�J�n������
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Client_Start( void )
{	
	// --- �ŏ���-1�ɂ��Ă���
	client_sned_work.mouse_point.x = -1;
	client_sned_work.mouse_point.y = -1;

	game_mode = GM_C_PLAY;

    return( TRUE );
}


/* ===============================================================================
|
|   GM_Client_End
|       �N���C�A���g�I������
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= */
int GM_Client_End( void )
{
	char str[ 256 ];

	if ( client_recv_work.black_cnt > client_recv_work.white_cnt )		// --- ���̏���
	{
		wsprintf( str, " %d �� %d �ō��̏����ł��B\n OK�{�^���ŃA�v���P�[�V�������I�����܂��B", client_recv_work.black_cnt, client_recv_work.white_cnt );
	}
	else if ( client_recv_work.black_cnt < client_recv_work.white_cnt )	// --- ���̏���
	{
		wsprintf( str, " %d �� %d �Ŕ��̏����ł��B\n OK�{�^���ŃA�v���P�[�V�������I�����܂��B", client_recv_work.black_cnt, client_recv_work.white_cnt );
	}
	else	// --- ��������
	{
		wsprintf( str, " %d �� %d �ň��������ł��B\n OK�{�^���ŃA�v���P�[�V�������I�����܂��B", client_recv_work.black_cnt, client_recv_work.white_cnt );
	}

	game_mode++;
	MessageBox( NULL, str, "Result", MB_OK );	
	
    return( TRUE );
}

/* ===============================================================================
|
|   Client_Disp_Show
|       �N���C�A���g�`�揈��
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> �E�B���h�E�n���h��
|   OUT --> int                 --> �I���l
+ ============================================================================= *
int Client_Disp_Show( HWND hWnd )
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
		if ( client_recv_work.osero_board_array[ i ] != OSERO_CHIP_NON )
		{
			TransparentBlt
			( 
				game_win.hScreenDC, 
				( ( i % OSERO_X_MAX ) * SPR_CHIP_CUT_W ) + DISP_BOARD_X + ( ( i % OSERO_X_MAX ) * CHIP_ADJUST_X ),
				( ( i / OSERO_Y_MAX ) * SPR_CHIP_CUT_H ) + DISP_BOARD_Y + ( ( i / OSERO_X_MAX ) * CHIP_ADJUST_Y ), 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				hDC, 
				( client_recv_work.osero_board_array[ i ] - 1 ) * SPR_CHIP_CUT_W, 0, 
				SPR_CHIP_CUT_W, SPR_CHIP_CUT_H, 
				RGB( 0, 255, 0 ) 
			);
		}
	}
/*
	// --- ���l���Z�b�g
	SelectObject( hDC, hBitmapSpr[ 2 ] );

	// --- PLAYER1�̃`�b�v���\��
	val1 = ( client_recv_work.chip_val[ PLYER_1P ] / 10 ) % 10;	// --- �\�̈�
	val2 = ( client_recv_work.chip_val[ PLYER_1P ] / 1 ) % 10;	// --- ��̈�

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
	val1 = ( client_recv_work.chip_val[ PLYER_2P ] / 10 ) % 10;	// --- �\�̈�
	val2 = ( client_recv_work.chip_val[ PLYER_2P ] / 1 ) % 10;	// --- ��̈�

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
|   ClientToServer_Send
|       �N���C�A���g����T�[�o�[�֑��M
+ --------------------------------------------------------------------------------
|   IN ---> NON
|   OUT --> int --> �I���l
+ ============================================================================= *
int ClientToServer_Send( void )
{
	// --- �f�[�^�̍쐬
	memcpy( client_sned_work.message_str, message_str, sizeof( message_str ) );						// --- ���b�Z�[�W������

	// --- �T�[�o�[�ɑ��M
	sendto( sock, ( const char * )&client_sned_work, sizeof( CLIENT_SEND_DATA ), 0, ( struct sockaddr * )&addr, sizeof( addr ) );	
	
    return( TRUE );
}


/* ===============================================================================
|
|   
|   
+ --------------------------------------------------------------------------------
|  
|  
+ ============================================================================= */
int GM_Client_Play(void)
{

	if (key_state[KEY_MOUSE_L] < 0) {
		mouse_point.x = (mouse_point.x - DISP_BOARD_X) / 
						(SPR_CHIP_CUT_W + CHIP_ADJUST_X);
		mouse_point.y = (mouse_point.y - DISP_BOARD_Y) / 
						(SPR_CHIP_CUT_H + CHIP_ADJUST_Y);

		if(mouse_point.x < OSERO_X_MAX && mouse_point.y < OSERO_Y_MAX &&
									mouse_point.x >= 0 && mouse_point.y >= 0) {

			client_sned_work.mouse_point.x = mouse_point.x;
			client_sned_work.mouse_point.y = mouse_point.y;
		}
		else
		{
			client_sned_work.mouse_point.x = -1;
			client_sned_work.mouse_point.y = -1;
		}

	}
	else
	{
		client_sned_work.mouse_point.x = -1;
		client_sned_work.mouse_point.y = -1;
	}

	if((key_state[KEY_F1] < 0) && GetWindowTextLength(edit_window) != 0) {

		GetWindowText( edit_window, message_str,sizeof(message_str) );
		SetWindowText( edit_window, "" );
	}

	SetWindowText( static_window,client_recv_work.message_str );

	if (client_recv_work.mode == 2) {
		game_mode = GM_C_END;
	}

	return(TRUE);

}





