
/*------------------------------------------------
|
|			
|
+--------------------------------------------------*/
#include "Common.h"

int SetStage_Init( void ){

	stagepos = VGet(0.0f,0.0f,0.0f) ;	//�X�e�[�W�̃|�W�V����
	if(CHOICESTAGE == 1){
		stagedata = MV1LoadModel("..\\Stage\\Stage00.mv1") ;
		// �V���h�E�}�b�v�n���h���̍쐬

		// ���C�g�̕�����ݒ�
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;

		// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;

		// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;

		stagedata_c = MV1LoadModel("..\\Stage\\Stage00_c.mv1") ;
		if(stagedata == -1) return -1 ;
	}
	else{
		stagedata = MV1LoadModel("..\\Stage\\Stage01.mv1") ;
		// �V���h�E�}�b�v�n���h���̍쐬

		// ���C�g�̕�����ݒ�
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;
		// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;
		// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;
		stagedata_c = MV1LoadModel("..\\Stage\\Stage01_c.mv1") ;
		if(stagedata == -1) return -1 ;	
	}
	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo( stagedata, -1 ) ;
	SetDrawScreen(DX_SCREEN_BACK) ;
	int MeshNum ;
	// ���f���Ɋ܂܂�郁�b�V���̐����擾����
    MeshNum = MV1GetMeshNum( stagedata ) ;
    // ���b�V���̐��������[�v
    for(int i = 0 ; i < MeshNum ; i ++ ){
	// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�̍ő���W�l��`��
		Position = MV1GetMeshMaxPosition( stagedata, i ) ;
		DrawFormatString( 0, 112, GetColor( 255,255,255 ), "Max Position          x %f  y %f  z %f", Position.x, Position.y, Position.z ) ;

		// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�̍ŏ����W�l��`��
		Position = MV1GetMeshMinPosition( stagedata, i ) ;
		DrawFormatString( 0, 128, GetColor( 255,255,255 ), "Min Position          x %f  y %f  z %f", Position.x, Position.y, Position.z ) ;
	}

	return 0 ;
}


