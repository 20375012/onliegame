
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
int Client_Game_Main( void )
//int Client_Game_Main( HWND hWnd )
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
	ClientToServer_Send	( );		// --- ���M����
	Client_Disp_Show	( );	// --- �`�揈��
    return( TRUE );
}



/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Client_Find( void )
{	
	if(	Chara_goblin_init() == -1)		return -1 ;
	if(	CameraPosition_Init()	== -1)	return -1 ;		// set position_camera
	if(	Chara_player_init() == -1)		return -1 ;

	// --- �T�[�o�[�ɑ��M��A�J�n��҂�
	if ( free_mode == 0 )
	{
		client_send_work.mode = 1;	// --- �T�[�o�[�ɒʒB
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
|
+ ============================================================================= */
int GM_Client_Start( void )
{	

	game_mode = GM_C_PLAY;

	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Client_Play( void )
{	
	if( Chara_goblin_Anim()== -1 ) return -1 ;
	if( Chara_goblin_fall() == -1) return -1 ;
	if( Chara_player_Anim() == -1 ) return -1 ;
	if( Chara_player_fall() == -1) return -1 ;

	Key_goblin();



	for ( int i = 0 ; i < 3 ; i++ )
	{
		gbcharainfo[i].pos.x += gbcharainfo[i].move.x ;
		gbcharainfo[i].pos.y += gbcharainfo[i].move.y ;
		gbcharainfo[i].pos.z += gbcharainfo[i].move.z ;
	}
	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Client_End( void )
{	
    return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int Client_Disp_Show( void )
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
int ClientToServer_Send( void )
{	
	
	memcpy( client_send_work.gbcharainfo, gbcharainfo, sizeof( gbcharainfo[0] ) );
	
	// --- �f�[�^�̍쐬
	memcpy( client_send_work.message_str, message_str, sizeof( message_str ) );		// --- ���b�Z�[�W������
	// --- �T�[�o�[�ɑ��M
	sendto( sock, ( const char * )&client_send_work, sizeof( CLIENT_SEND_DATA ), 0, ( struct sockaddr * )&addr, sizeof( addr ) );	
	
    return( TRUE );    return( TRUE );
}




