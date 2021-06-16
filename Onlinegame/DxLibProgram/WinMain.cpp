
#include "Common.h"		// --- �S�t�@�C���ł̋��ʃw�b�_�[

#define CHOICESTAGE 1

int WINAPI WinMain(HINSTANCE hI,HINSTANCE hP,LPSTR lpC,int nC){
	
	int anim_nutral,anim_run,anim_jumpin,anim_jumploop,anim_jumpout,anim_attack,anim_attack2,anim_attack3 ;
	int gbanim_nutral,gbanim_damage ;

	int stagedata ;
	int stagedata_c ; 

	char BGMO_FilePath[] = "BGM_stg0.ogg" ;
	char String[256] ;
	int BGMSoundHandle ;
	int BGMLoopStartPosition = -1 ;
	int BGMLoopEndPosition = -1 ;
	char SE_FilePath[] = "dmg_byAxe_00.wav" ;
	int SESoundHandle ;


	// �S�̓I�ȃQ�[���̊Ǘ�
	int gamemode = eSceneBlank ;

	//�X�e�[�W�̃|�W�V����
	VECTOR stagepos = VGet(0.0f,0.0f,0.0f) ;

	// �V���h�E�}�b�v�n���h��
	int ShadowMapHandle ;

	VECTOR cpos,ctgt ;
	// �J�����|�W�V���� cpos:�J�����ʒu�@ctgt:�J���������_
	if(CHOICESTAGE == 1){
		cpos = VGet(0.0f,1400.0f,-1500.0f) ;
		ctgt = VGet(0.0f,500.0f,-400.0f) ;
	}
	else{
		cpos = VGet(500.0f,1501.0f,1800.0f) ;
		ctgt = VGet(500.0f,800.0f,2800.0f) ; // ���A
	}

	// �L�������
	SCharaInfo charainfo[5] ;

	// �X�e�[�W�R���W�������
	MV1_COLL_RESULT_POLY_DIM HitDim ;					// �L�����N�^�[�̎��͂ɂ���|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	int WallNum ;										// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int FloorNum ;										// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��

	int HitFlag ;										// �|���S���ɓ����������ǂ������L�����Ă����̂Ɏg���ϐ�( �O:�������Ă��Ȃ�  �P:�������� )
	MV1_COLL_RESULT_POLY *Poly ;						// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
	HITRESULT_LINE LineRes ;							// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����


	// �L�������q�b�g�������̃|���S���\���̍��W
	VECTOR PolyCharaHitField[3] ;

	int key ;

	VECTOR Position ;


	ChangeWindowMode(TRUE) ;

	SetGraphMode(900,600,32) ;

	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	// DX���C�u�����̏�����
	if(DxLib_Init() == -1) return -1 ;

	// �A�j���[�V�����̓ǂݍ���
	anim_nutral = MV1LoadModel("..\\Player\\Anim_Neutral.mv1") ;	// �����A�j��
	if(anim_nutral == -1) return -1 ;
	
	anim_run = MV1LoadModel("..\\Player\\Anim_Run.mv1") ;			// ����A�j��	
	if(anim_run == -1) return -1 ;

	anim_jumpin = MV1LoadModel("..\\Player\\Anim_Jump_In.mv1") ;		// �W�����v����n�߃A�j��
	if(anim_jumpin == -1) return -1 ;
	anim_jumploop = MV1LoadModel("..\\Player\\Anim_Jump_Loop.mv1") ;	// �W�����v���[�v�A�j��
	if(anim_jumploop == -1) return -1 ;
	anim_jumpout = MV1LoadModel("..\\Player\\Anim_Jump_Out.mv1") ;		// �W�����v���n�A�j��
	if(anim_jumpout == -1) return -1 ;
	anim_attack = MV1LoadModel("..\\Player\\Anim_Attack1.mv1") ;		// �A�^�b�N�A�j��
	if(anim_attack == -1) return -1 ;
	anim_attack2 = MV1LoadModel("..\\Player\\Anim_Attack2.mv1") ;		// �A�^�b�N�A�j��2
	if(anim_attack2 == -1) return -1 ;
	anim_attack3 = MV1LoadModel("..\\Player\\Anim_Attack3.mv1") ;		// �A�^�b�N�A�j��2
	if(anim_attack3 == -1) return -1 ;	
	gbanim_nutral = MV1LoadModel("..\\Goblin\\Anim_Neutral.mv1") ;		// 
	if(gbanim_nutral == -1) return -1 ;
	gbanim_damage = MV1LoadModel("..\\Goblin\\Anim_Damage.mv1") ;		//
	if(gbanim_damage == -1) return -1 ;


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


	// ------------------------------------------------------------------------ // 
	//							�L�������Z�b�g								//
	// ------------------------------------------------------------------------ //
	if(CHOICESTAGE == 1){
		charainfo[0].pos = VGet(0.0f,500.0f,0.0f) ;		 // ����
	}
	else{
		charainfo[0].pos = VGet(500.0f,800.0f,2800.0f) ; // ���A
	}
	charainfo[0].mode = STAND ;
	charainfo[0].direction = DOWN ;
	charainfo[0].charahitinfo.Height = PC_HEIGHT ;
	charainfo[0].charahitinfo.Width = PC_WIDTH ;
	charainfo[0].charahitinfo.CenterPosition = charainfo[0].pos ;
	charainfo[0].move = VGet(0.0f,0.0f,0.0f) ;

	// --------------------------- // 
	//		�L�������f���Z�b�g	   //
	// --------------------------- // 
	charainfo[0].model1 = MV1LoadModel("..\\Player\\PC.mv1") ;
	if(charainfo[0].model1 == -1) return -1 ;
	charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
	charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
	// �A�j���[�V�������ē����Ă����̏�œ����Ă�悤�ȏ��
	charainfo[0].rootflm = MV1SearchFrame(charainfo[0].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[0].model1,charainfo[0].rootflm,MGetIdent()) ;
	charainfo[0].playtime = 0.0f ;

	// ------------------------------------------------------------------------ // 
	//							�L�������Z�b�g								//
	// ------------------------------------------------------------------------ // 
	charainfo[1].pos = VGet(1000.0f,0.0f,-400.0f) ;
	charainfo[1].direction = DOWN ;
	charainfo[1].charahitinfo.Height = PC_HEIGHT ;
	charainfo[1].charahitinfo.Width = PC_WIDTH ;
	charainfo[1].charahitinfo.CenterPosition = charainfo[1].pos ;
	charainfo[1].move = VGet(0.0f,0.0f,0.0f) ;
//	--------------------------------------------------------------------------------
	charainfo[2].mode = GSTAND ;
	charainfo[2].pos = VGet(800.0f,0.0f,-400.0f) ;
	charainfo[2].direction = DOWN ;
	charainfo[2].charahitinfo.Height = PC_HEIGHT ;
	charainfo[2].charahitinfo.Width = PC_WIDTH ;
	charainfo[2].charahitinfo.CenterPosition = charainfo[2].pos ;
	charainfo[2].move = VGet(0.0f,0.0f,0.0f) ;


	// --------------------------- // 
	//		�L�������f���Z�b�g	   //
	// --------------------------- // 
	charainfo[1].model1 = MV1DuplicateModel(charainfo[0].model1) ;
	if(charainfo[1].model1 == -1) return -1 ;
	charainfo[1].attachidx = MV1AttachAnim(charainfo[1].model1,0,anim_nutral) ;
	charainfo[1].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[1].model1,charainfo[1].attachidx) ;
	// �A�j���[�V�������ē����Ă����̏�œ����Ă�悤�ȏ��
	charainfo[1].rootflm = MV1SearchFrame(charainfo[1].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[1].model1,charainfo[1].rootflm,MGetIdent()) ;
	charainfo[1].playtime = 0.0f ;
//--------------------------------------------------------------------------------------------------------------
	charainfo[2].model1 = MV1LoadModel("..\\Goblin\\Goblin.mv1") ;
	if(charainfo[2].model1 == -1) return -1 ;
	charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_nutral) ;
	charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
	// �A�j���[�V�������ē����Ă����̏�œ����Ă�悤�ȏ��
	charainfo[2].rootflm = MV1SearchFrame(charainfo[2].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[2].model1,charainfo[2].rootflm,MGetIdent()) ;
	charainfo[2].playtime = 0.0f ;

	// �X�e�[�W���̓ǂݍ���
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

		PlaySoundMem( BGMSoundHandle, BGMLoopStartPosition >= 0 ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK) ;
		PlaySoundMem( SESoundHandle, DX_PLAYTYPE_NORMAL) ;


	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){

		PolyCharaHitField[0] = VGet(0.0f,0.0f,0.0f) ;
		PolyCharaHitField[1] = VGet(0.0f,0.0f,0.0f) ;
		PolyCharaHitField[2] = VGet(0.0f,0.0f,0.0f) ;
						
		switch(gamemode){
			case eSceneBlank :
				gamemode = eScenePlay ;
				break ;
			case eScenePlay :

				// �A�j���[�V�����i�s
				if(charainfo[0].mode != JUMPOUT){
					charainfo[0].playtime += 0.5f ;
					charainfo[2].playtime += 0.5f ;
				}
				else{
					charainfo[0].playtime += 1.0f ;	
					charainfo[2].playtime += 1.0f ;
				}

				//if(charainfo[0].mode == ATTACK){
				//	charainfo[0].playtime += 1.0f ;
				//}

				if(charainfo[0].mode != JUMPLOOP && charainfo[0].mode != FALL){
					if(charainfo[0].playtime > charainfo[0].anim_totaltime){


						if(charainfo[0].mode == JUMPIN){
							MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_jumploop) ;
							charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].mode = JUMPLOOP ;
							charainfo[0].move.y = 15.0f ;
							charainfo[0].pos.y += charainfo[0].move.y ; //�W�����v����̒n�ʂ߂荞�݂�����邽��
						}
						
						if(charainfo[0].mode == JUMPOUT){
							MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
							charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].mode = STAND ;
						}

						if(charainfo[0].mode == ATTACK){
							MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
							charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].mode = STAND ;

						}

						if(charainfo[0].mode == ATTACK2){
							MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
							charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].mode = STAND ;

						}

						if(charainfo[0].mode == ATTACK3){
							MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
							charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
							charainfo[0].mode = STAND ;

						}


						charainfo[0].playtime = 0.0f ;
					}
				}

				if( charainfo[2].mode != FALL){
					if(charainfo[2].playtime > charainfo[2].anim_totaltime){
						if(charainfo[2].mode == STAND )
						{
							MV1DetachAnim(charainfo[2].model1,charainfo[2].attachidx) ;
							charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_nutral) ;
							charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
						}
						if(charainfo[2].mode == GDAMAGE){
							MV1DetachAnim(charainfo[2].model1,charainfo[2].attachidx) ;
							charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_nutral) ;
							charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
							charainfo[2].mode = STAND ;
						}

						charainfo[2].playtime = 0.0f ;

					}
				}

				MV1SetAttachAnimTime(charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
				MV1SetAttachAnimTime(charainfo[2].model1,charainfo[2].attachidx,charainfo[2].playtime) ;

				// �L�[����
				if(charainfo[0].mode == STAND || charainfo[0].mode == RUN ){
					key = GetJoypadInputState(DX_INPUT_KEY_PAD1) ;
					if(key & PAD_INPUT_DOWN){
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.z = -6.0f ; // �������� ��O�Ɉړ�
						charainfo[0].direction = DOWN ;
					}

					if(key & PAD_INPUT_UP){
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.z = 6.0f ; // ������� ���Ɉړ�
						charainfo[0].direction = UP ;
					}
					if(key & PAD_INPUT_LEFT){
						charainfo[0].move.x = -6.0f ; // �������� ���Ɉړ�
						charainfo[0].move.z = 0.0f ;
						charainfo[0].direction = LEFT ;

						StopSoundMem( BGMSoundHandle ) ;
					}
					if(key & PAD_INPUT_RIGHT){
						charainfo[0].move.x = 6.0f ; // �E������ �E�Ɉړ�
						charainfo[0].move.z = 0.0f ;
						charainfo[0].direction = RIGHT ;
					}
					if (key & PAD_INPUT_7) {
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx);
						charainfo[0].mode = JUMPIN ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_jumpin);
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(
							charainfo[0].model1,charainfo[0].attachidx);
						charainfo[0].playtime = 0.0f ;
						MV1SetAttachAnimTime(
							charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
					}

					if (key & PAD_INPUT_A) {
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx);
						charainfo[0].mode = ATTACK ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_attack) ;
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(
							charainfo[0].model1,charainfo[0].attachidx);
						charainfo[0].playtime = 0.0f ;
						MV1SetAttachAnimTime(
							charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.y = 0.0f ;
						charainfo[0].move.z = 0.0f ;

					}

					if (key & PAD_INPUT_B) {
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].mode = ATTACK2 ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_attack2) ;
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(
							charainfo[0].model1,charainfo[0].attachidx);
						charainfo[0].playtime = 0.0f ;
						MV1SetAttachAnimTime(
							charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.y = 0.0f ;
						charainfo[0].move.z = 0.0f ;

					}

					if (key & PAD_INPUT_C) {
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx);
						charainfo[0].mode = ATTACK3 ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_attack3) ;
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(
							charainfo[0].model1,charainfo[0].attachidx);
						charainfo[0].playtime = 0.0f ;
						MV1SetAttachAnimTime(
							charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.y = 0.0f ;
						charainfo[0].move.z = 0.0f ;

					}

				}
				if(key == 0){
					if(charainfo[0].mode == RUN){
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.y = 0.0f ;
						charainfo[0].move.z = 0.0f ;
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].mode = STAND ;
					}
				}
				else{
					if(charainfo[0].mode == STAND){
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_run) ;
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].mode = RUN ;
					}
				}


				//if (charainfo[0].mode != JUMPLOOP && charainfo[0].mode != FALL) {
				//	if(charainfo[0].playtime > charainfo[0].anim_totaltime) {
				//		if (charainfo[0].mode = JUMPIN) {
				//			MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx);
				//			charainfo[0].attachidx = MV1AttachAnim(
				//				charainfo[0].model1,0,anim_jumploop) ;
				//			charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(
				//				charainfo[0].model1,charainfo[0].attachidx) ;
				//			charainfo[0].mode = JUMPLOOP ;
				//			charainfo[0].move.y = 15.0f ;

				//			charainfo[0].pos.y += charainfo[0].move.y ;
				//		}
				//	}

				//}

				//if (charainfo[0].mode == JUMPIN) {
				//	MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
				//	charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_jumploop) ;
				//	charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(
				//		charainfo[0].model1,charainfo[0].attachidx) ;
				//	charainfo[0].mode = JUMPLOOP ;
				//	charainfo[0].move.y = 15.0f ;


				//	charainfo[0].pos.y += charainfo[0].move.y ;
				//}

				ClearDrawScreen() ;
				// �����ɐF�X������ǉ�����
				DrawBox(0,0,900,600,GetColor(255,255,255),true) ; //�Ō�̈�����false�ɂ���Ɠh��Ԃ�����

				// �L�����ƃq�b�g�`�F�b�N
				if(HitCheck_Capsule_Capsule(VAdd(charainfo[0].pos,charainfo[0].move),VAdd(charainfo[0].pos,charainfo[0].move),charainfo[0].charahitinfo.Width / 2,
											VAdd(charainfo[1].pos,charainfo[1].move),VAdd(charainfo[1].pos,charainfo[1].move),charainfo[1].charahitinfo.Width / 2) == TRUE){
					// �ړ��ʂ𖳂���
					charainfo[0].move.x = 0.0f ;
					charainfo[0].move.y = 0.0f ;
					charainfo[0].move.z = 0.0f ;
				}

				if(HitCheck_Capsule_Capsule(VAdd(charainfo[0].pos,charainfo[0].move),VAdd(charainfo[0].pos,charainfo[0].move),charainfo[0].charahitinfo.Width / 2,
											VAdd(charainfo[2].pos,charainfo[2].move),VAdd(charainfo[2].pos,charainfo[2].move),charainfo[2].charahitinfo.Width / 2) == TRUE){
					// �ړ��ʂ𖳂���
					charainfo[0].move.x = 0.0f ;
					charainfo[0].move.y = 0.0f ;
					charainfo[0].move.z = 0.0f ;
				}
			if( (charainfo[0].direction == DOWN && charainfo[2].pos.z < charainfo[0].pos.z)||(charainfo[0].direction == UP && charainfo[2].pos.z > charainfo[0].pos.z)||(charainfo[0].direction == LEFT && charainfo[2].pos.x < charainfo[0].pos.x)||(charainfo[0].direction == RIGHT && charainfo[2].pos.x > charainfo[0].pos.x)){
					if((charainfo[0].mode == ATTACK) &&(charainfo[0].playtime == 3.0f)){
							if(HitCheck_Capsule_Capsule(VAdd(charainfo[0].pos,charainfo[0].move),VAdd(charainfo[0].pos,charainfo[0].move),charainfo[0].charahitinfo.Width + 4,
														VAdd(charainfo[2].pos,charainfo[2].move),VAdd(charainfo[2].pos,charainfo[2].move),charainfo[2].charahitinfo.Width + 4) == TRUE){

								charainfo[2].move.x = 0.0f ;
								charainfo[2].move.y = 0.0f ;
								charainfo[2].move.z = 0.0f ;

								charainfo[2].mode = GDAMAGE ;
								if(charainfo[2].mode == GDAMAGE){
									MV1DetachAnim(charainfo[2].model1,charainfo[2].attachidx) ;
									charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_damage) ;
									charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
									charainfo[2].playtime = 0.0f ;
								}
								// �ړ��ʂ𖳂���

							}


					}

					if((charainfo[0].mode == ATTACK2) &&(charainfo[0].playtime == 3.0f)){
							if(HitCheck_Capsule_Capsule(VAdd(charainfo[0].pos,charainfo[0].move),VAdd(charainfo[0].pos,charainfo[0].move),charainfo[0].charahitinfo.Width + 4,
														VAdd(charainfo[2].pos,charainfo[2].move),VAdd(charainfo[2].pos,charainfo[2].move),charainfo[2].charahitinfo.Width + 4) == TRUE){

								charainfo[2].move.x = 0.0f ;
								charainfo[2].move.y = 0.0f ;
								charainfo[2].move.z = 0.0f ;

								charainfo[2].mode = GDAMAGE ;
								if(charainfo[2].mode == GDAMAGE){
									MV1DetachAnim(charainfo[2].model1,charainfo[2].attachidx) ;
									charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_damage) ;
									charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
									charainfo[2].playtime = 0.0f ;
								}
								// �ړ��ʂ𖳂���

							}


					}
					if((charainfo[0].mode == ATTACK3) &&(charainfo[0].playtime == 3.0f)){
							if(HitCheck_Capsule_Capsule(VAdd(charainfo[0].pos,charainfo[0].move),VAdd(charainfo[0].pos,charainfo[0].move),charainfo[0].charahitinfo.Width + 4,
														VAdd(charainfo[2].pos,charainfo[2].move),VAdd(charainfo[2].pos,charainfo[2].move),charainfo[2].charahitinfo.Width + 4) == TRUE){

								charainfo[2].move.x = 0.0f ;
								charainfo[2].move.y = 0.0f ;
								charainfo[2].move.z = 0.0f ;

								charainfo[2].mode = GDAMAGE ;
								if(charainfo[2].mode == GDAMAGE){
									MV1DetachAnim(charainfo[2].model1,charainfo[2].attachidx) ;
									charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_damage) ;
									charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
									charainfo[2].playtime = 0.0f ;
								}
								// �ړ��ʂ𖳂���

							}


					}


			}

				HitDim = MV1CollCheck_Sphere( stagedata, -1, charainfo[0].pos, CHARA_ENUM_DEFAULT_SIZE + VSize( charainfo[0].move ) ) ;
				WallNum = 0 ;
				FloorNum = 0 ;
				// ���o���ꂽ�|���S���̐������J��Ԃ�
				for(int i = 0 ; i < HitDim.HitNum ; i ++ ){
					// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
					if( HitDim.Dim[i].Normal.y < 0.000001f && HitDim.Dim[i].Normal.y > -0.000001f ){
						printf("�ǈ���\n") ;
						// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�L�����N�^�[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
						if( HitDim.Dim[i].Position[0].y > charainfo[0].pos.y + 1.0f ||
							HitDim.Dim[i].Position[1].y > charainfo[0].pos.y + 1.0f ||
							HitDim.Dim[i].Position[2].y > charainfo[0].pos.y + 1.0f ){
							// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
							if( WallNum < CHARA_MAX_HITCOLL ){
								// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
								Wall[WallNum] = &HitDim.Dim[i] ;

								// �ǃ|���S���̐������Z����
								WallNum ++ ;
							}
						}
					}
					else{
						// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
						if( FloorNum < CHARA_MAX_HITCOLL ){
							// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
							Floor[FloorNum] = &HitDim.Dim[i] ;

							// ���|���S���̐������Z����
							FloorNum ++ ;
						}
					}
				}
				float MaxY ;
				float MaxY_poly ;
				// ���|���S���Ƃ̓����蔻��
				if( FloorNum != 0 ){
					// ���|���S���ɓ����������ǂ����̃t���O��|���Ă���
					HitFlag = 0 ;
					// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
					MaxY = 0.0f ;
					MaxY_poly = 0.0f ;

					// ���|���S���̐������J��Ԃ�
					for(int i = 0 ; i < FloorNum ; i ++ ){
						// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
						Poly = Floor[i] ;

						VECTOR cal_pos1 = VAdd( charainfo[0].pos, VGet( 0.0f, PC_HEIGHT, 0.0f ) ) ;
						VECTOR cal_pos2 = VAdd( charainfo[0].pos, VGet( 0.0f, -5.0f, 0.0f ) ) ;
						// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
						LineRes = HitCheck_Line_Triangle( cal_pos1,cal_pos2,Poly->Position[0], Poly->Position[1], Poly->Position[2] ) ;
						
						// �������Ă��Ȃ������牽�����Ȃ�
						if( LineRes.HitFlag == TRUE){
							PolyCharaHitField[0] = Poly->Position[0] ;
							PolyCharaHitField[1] = Poly->Position[1] ;
							PolyCharaHitField[2] = Poly->Position[2] ;
						}
						else{
							continue ;
						}

						// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
						if( HitFlag == 1 && MaxY > LineRes.Position.y ){
							continue ;
						}

						// �|���S���ɓ��������t���O�𗧂Ă�
						HitFlag = 1 ;

						// �ڐG�����x���W��ۑ�����
						MaxY = LineRes.Position.y ;
						MaxY_poly = Poly->Position[1].y ;
					}
				}
				// ���|���S���ɓ����������ǂ����ŏ����𕪊�
				if( HitFlag == 1 ){
					// �ڐG�����|���S���ň�ԍ����x���W���L�����N�^�[�̂x���W�ɂ���
					charainfo[0].pos.y = MaxY ;
					charainfo[0].move.y = 0.0f ;

					if(charainfo[0].mode == JUMPLOOP || charainfo[0].mode == FALL){
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].mode = JUMPOUT ;
						charainfo[0].playtime = 0.0f ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_jumpout) ;
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.y = 0.0f ;
						charainfo[0].move.z = 0.0f ;
					}
				}
				else{
					// ���ɕ�������� 
					if(charainfo[0].mode != FALL){ // �W�����v��Ԃ���Ȃ�
						MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].mode = FALL ;
						charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_jumploop) ;
						charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
						charainfo[0].playtime = 7.0f ;
						MV1SetAttachAnimTime(charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
					}
				}

				// �W�����v����������d�͒ǉ�������
				if(charainfo[0].mode == FALL){
					charainfo[0].move.y -= 0.5f ;
				}

				// ���o�����L�����N�^�[�̎��͂̃|���S�������J������
				MV1CollResultPolyDimTerminate( HitDim ) ;

				// �ړ�����
				charainfo[0].pos.x += charainfo[0].move.x ;
				charainfo[0].pos.y += charainfo[0].move.y ;
				charainfo[0].pos.z += charainfo[0].move.z ;

				cpos.x += charainfo[0].move.x ;
				cpos.y += charainfo[0].move.y ;
				cpos.z += charainfo[0].move.z ;

				ctgt.x += charainfo[0].move.x ;
				ctgt.y += charainfo[0].move.y ;
				ctgt.z += charainfo[0].move.z ;

				SetCameraPositionAndTargetAndUpVec(cpos,ctgt,VGet(0.0f,0.0f,1.0f)) ;

				// ���f���̉�]
				MV1SetRotationXYZ(charainfo[0].model1,VGet(0.0f,1.57f * charainfo[0].direction,0.0f)) ;
				// ���f���̈ړ�(�z�u)
				MV1SetPosition(charainfo[0].model1,charainfo[0].pos);

				// ���f���̉�]
				MV1SetRotationXYZ(charainfo[1].model1,VGet(0.0f,1.57f * charainfo[1].direction,0.0f)) ;
				// ���f���̈ړ�(�z�u)
				MV1SetPosition(charainfo[1].model1,charainfo[1].pos);

				// ���f���̉�]
				MV1SetRotationXYZ(charainfo[2].model1,VGet(0.0f,1.57f * charainfo[2].direction,0.0f)) ;
				// ���f���̈ړ�(�z�u)
				MV1SetPosition(charainfo[2].model1,charainfo[2].pos);

				// �n��(�z�u)
				MV1SetPosition(stagedata,stagepos);

				// �V���h�E�}�b�v�ւ̕`��̏���
				ShadowMap_DrawSetup( ShadowMapHandle ) ;

				// ���f���̕`�� - �e�̕`��
				for ( int i = 0 ; i < 3 ; i++ )
				{
					MV1DrawModel(charainfo[i].model1) ;
					//MV1DrawModel(charainfo[1].model1) ;
				}
				// �n�ʕ`��
//				MV1DrawModel(stagedata) ;				

				// �V���h�E�}�b�v�ւ̕`����I��
				ShadowMap_DrawEnd() ;

				// �`��Ɏg�p����V���h�E�}�b�v��ݒ�
				SetUseShadowMap( 0, ShadowMapHandle ) ;


				// �n��(�z�u)���`��
				MV1DrawModel(stagedata) ;

				DrawTriangle3D(PolyCharaHitField[0], PolyCharaHitField[1], PolyCharaHitField[2],
				GetColor( 255,0,0 ), TRUE ) ;

				// ���f���̕`��
				for ( int i = 0 ; i < 3 ; i++ )
				{
					MV1DrawModel(charainfo[i].model1) ;
					//MV1DrawModel(charainfo[1].model1) ;
				}
				// �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
				SetUseShadowMap( 0, -1 ) ;

				ScreenFlip() ;
				break ;
			case eSceneClear :
				break ;
		}

	}


	// �V���h�E�}�b�v�̍폜
	DeleteShadowMap( ShadowMapHandle ) ;
	// �X�e�[�W���f���̍폜
	MV1DeleteModel( stagedata ) ;
	// �L�����N�^�[���f���̍폜
	MV1DeleteModel( charainfo[0].model1 ) ;
	MV1DeleteModel( charainfo[1].model1 ) ;

	DxLib_End() ;
	return 0 ;
}



