
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
int Server_Game_Main( void )
//int Server_Game_Main( HWND hWnd )
{	
	// --- �Q�[�����[�h����
	switch( game_mode )
	{
		case GM_S_WAIT:		GM_Server_Wait	( );		break;	// --- �N���C�A���g�҂�
		case GM_S_START:	GM_Server_Start	( );		break;	// --- �X�^�[�g
		case GM_S_PLAY:		GM_Server_Play	( );		break;	// --- �ΐ풆
		case GM_S_END:		GM_Server_End	( );		break;	// --- �I��
	}


	ServerToClient_Send	( );		// --- ���M����
	Server_Disp_Show	( );		//	---	�`��
	return( TRUE );
}

/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Wait( void )
{	
	if(	Chara_player_init() == -1)		return -1 ;
	if(	CameraPosition_Init()	== -1)	return -1 ;		// set position_camera
	if(	Chara_goblin_init() == -1)		return -1 ;

	// --- �A�h���X���i�[����Ă�����
	if ( recv_addr.sin_addr.s_addr != 0 )
	{
		game_mode = GM_S_START;			// --- �Q�[���J�n
		server_send_work.mode = 1;		// --- �Q�[���J�n�f�[�^�i�[
	}
	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Start( void )
{	
 
	game_mode = GM_S_PLAY;

	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Play( void )
{	
//	GM_Server_Play_1P( );	// --- �T�[�o�[1P����
//	GM_Server_Play_2P( );	// --- �T�[�o�[2P����

	if( Chara_player_Anim() == -1 ) return -1 ;
	if( Chara_player_fall() == -1) return -1 ;
	if( Chara_goblin_Anim()== -1 ) return -1 ;
	if( Chara_goblin_fall() == -1) return -1 ;

	Key_player();

	for ( int i = 0 ; i < 3 ; i++ )
	{
		charainfo[i].pos.x += charainfo[i].move.x ;
		charainfo[i].pos.y += charainfo[i].move.y ;
		charainfo[i].pos.z += charainfo[i].move.z ;
	}

	if ( Finalize_Check( ) == TRUE )
	{
		server_send_work.mode = 2;		// --- �Q�[���I���f�[�^�i�[
		game_mode = GM_S_END;
	}
	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Play_1P( void )
{
	return TRUE ;
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Play_2P( void )
{
	return TRUE ;
}

/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_End( void )
{	
    return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int Server_Disp_Show( void )
{

	ClearDrawScreen() ;


	//CAMERA
	if( CameraPosition() == -1) return -1 ;


	for ( int i = 0 ; i < 3 ; i++ )
	{
		MV1SetRotationXYZ(charainfo[i].model1,VGet(0.0f,1.57f * charainfo[0].direction,0.0f)) ;					// ���f���̉�]
		MV1SetPosition(charainfo[i].model1,charainfo[i].pos);													// ���f���̈ړ�(�z�u)
		MV1SetRotationXYZ(gbcharainfo[i].model1,VGet(0.0f,1.57f * gbcharainfo[0].direction,0.0f)) ;					// ���f���̉�]
		MV1SetPosition(gbcharainfo[i].model1,gbcharainfo[i].pos);													// ���f���̈ړ�(�z�u)
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

	
    return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int ServerToClient_Send( void )
{

	memcpy( server_send_work.charainfo, charainfo, sizeof( charainfo[0] ) );
//	server_send_work.stagedata = stagedata ;
//	server_send_work.stagepos = stagepos ;
//	server_send_work.ShadowMapHandle = ShadowMapHandle ;

	// --- �f�[�^�̍쐬
	memcpy( server_send_work.message_str, message_str, sizeof( message_str ) );		// --- ���b�Z�[�W������
	// --- �N���C�A���g�ɑ��M
	sendto( sock, ( const char * )&server_send_work, sizeof( SERVER_SEND_DATA ), 0, ( struct sockaddr * )&recv_addr, sizeof( recv_addr ) );	

    return( TRUE );
}


