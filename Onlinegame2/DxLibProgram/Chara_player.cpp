
/*------------------------------------------------
|
|
+--------------------------------------------------*/
#include "Common.h"

/*------------------------------------------------
|
|
+--------------------------------------------------*/
int Chara_player_init( void ){

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


	return 0;
}

/*------------------------------------------------
|
|
+--------------------------------------------------*/
int Chara_goblin_init( void ){

	gbanim_nutral = MV1LoadModel("..\\Goblin\\Anim_Neutral.mv1") ;		// 
	if(gbanim_nutral == -1) return -1 ;
	gbanim_damage = MV1LoadModel("..\\Goblin\\Anim_Damage.mv1") ;		//
	if(gbanim_damage == -1) return -1 ;

	return 0;
}

/*------------------------------------------------
|
|
+--------------------------------------------------*/
int Chara_player_fall( void ){
	charainfo[0].HitDim = MV1CollCheck_Sphere( stagedata, -1, charainfo[0].pos, CHARA_ENUM_DEFAULT_SIZE + VSize( charainfo[0].move ) ) ;
	charainfo[0].WallNum = 0 ;
	charainfo[0].FloorNum = 0 ;
	// ���o���ꂽ�|���S���̐������J��Ԃ�
	for(int i = 0 ; i < charainfo[0].HitDim.HitNum ; i ++ ){
		// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
		if( charainfo[0].HitDim.Dim[i].Normal.y < 0.000001f && charainfo[0].HitDim.Dim[i].Normal.y > -0.000001f ){
			printf("�ǈ���\n") ;
			// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�L�����N�^�[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
			if( charainfo[0].HitDim.Dim[i].Position[0].y > charainfo[0].pos.y + 1.0f ||
				charainfo[0].HitDim.Dim[i].Position[1].y > charainfo[0].pos.y + 1.0f ||
				charainfo[0].HitDim.Dim[i].Position[2].y > charainfo[0].pos.y + 1.0f ){
				// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if( charainfo[0].WallNum < CHARA_MAX_HITCOLL ){								
					Wall[charainfo[0].WallNum] = &charainfo[0].HitDim.Dim[i] ;	// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
					charainfo[0].WallNum ++ ;						// �ǃ|���S���̐������Z����
				}
			}
		}
		else{
			// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
			if( charainfo[0].FloorNum < CHARA_MAX_HITCOLL ){							
				Floor[charainfo[0].FloorNum] = &charainfo[0].HitDim.Dim[i] ;		// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
				charainfo[0].FloorNum ++ ;							// ���|���S���̐������Z����
			}
		}
	}
	float MaxY ;
	float MaxY_poly ;
	// ���|���S���Ƃ̓����蔻��
	if( charainfo[0].FloorNum != 0 ){
		// ���|���S���ɓ����������ǂ����̃t���O��|���Ă���
		charainfo[0].HitFlag = 0 ;
		// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
		MaxY = 0.0f ;
		MaxY_poly = 0.0f ;

		// ���|���S���̐������J��Ԃ�
		for(int i = 0 ; i < charainfo[0].FloorNum ; i ++ ){
			// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
			Poly = Floor[i] ;

			VECTOR cal_pos1 = VAdd( charainfo[0].pos, VGet( 0.0f, PC_HEIGHT, 0.0f ) ) ;
			VECTOR cal_pos2 = VAdd( charainfo[0].pos, VGet( 0.0f, -5.0f, 0.0f ) ) ;
			// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
			LineRes = HitCheck_Line_Triangle( cal_pos1,cal_pos2,Poly->Position[0], Poly->Position[1], Poly->Position[2] ) ;
						
			// �������Ă��Ȃ������牽�����Ȃ�
			if( LineRes.HitFlag == TRUE){
				charainfo[0].PolyCharaHitField[0] = Poly->Position[0] ;
				charainfo[0].PolyCharaHitField[1] = Poly->Position[1] ;
				charainfo[0].PolyCharaHitField[2] = Poly->Position[2] ;
			}
			else{
				continue ;
			}

			// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
			if( charainfo[0].HitFlag == 1 && MaxY > LineRes.Position.y ){
				continue ;
			}

			// �|���S���ɓ��������t���O�𗧂Ă�
			charainfo[0].HitFlag = 1 ;

			// �ڐG�����x���W��ۑ�����
			MaxY = LineRes.Position.y ;
			MaxY_poly = Poly->Position[1].y ;
		}
	}
	// ���|���S���ɓ����������ǂ����ŏ����𕪊�
	if( charainfo[0].HitFlag == 1 ){
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
	MV1CollResultPolyDimTerminate( charainfo[0].HitDim ) ;
			return 0;
}

/*------------------------------------------------
|
|
+--------------------------------------------------*/
int Chara_goblin_fall( void ){
	gbcharainfo[0].HitDim = MV1CollCheck_Sphere( stagedata, -1, gbcharainfo[0].pos, CHARA_ENUM_DEFAULT_SIZE + VSize( gbcharainfo[0].move ) ) ;
	gbcharainfo[0].WallNum = 0 ;
	gbcharainfo[0].FloorNum = 0 ;
	// ���o���ꂽ�|���S���̐������J��Ԃ�
	for(int i = 0 ; i < gbcharainfo[0].HitDim.HitNum ; i ++ ){
		// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
		if( gbcharainfo[0].HitDim.Dim[i].Normal.y < 0.000001f && gbcharainfo[0].HitDim.Dim[i].Normal.y > -0.000001f ){
			printf("�ǈ���\n") ;
			// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�L�����N�^�[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
			if( gbcharainfo[0].HitDim.Dim[i].Position[0].y > gbcharainfo[0].pos.y + 1.0f ||
				gbcharainfo[0].HitDim.Dim[i].Position[1].y > gbcharainfo[0].pos.y + 1.0f ||
				gbcharainfo[0].HitDim.Dim[i].Position[2].y > gbcharainfo[0].pos.y + 1.0f ){
				// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if( gbcharainfo[0].WallNum < CHARA_MAX_HITCOLL ){								
					Wall[gbcharainfo[0].WallNum] = &gbcharainfo[0].HitDim.Dim[i] ;	// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
					gbcharainfo[0].WallNum ++ ;						// �ǃ|���S���̐������Z����
				}
			}
		}
		else{
			// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
			if( gbcharainfo[0].FloorNum < CHARA_MAX_HITCOLL ){							
				Floor[gbcharainfo[0].FloorNum] = &gbcharainfo[0].HitDim.Dim[i] ;		// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
				gbcharainfo[0].FloorNum ++ ;							// ���|���S���̐������Z����
			}
		}
	}
	float MaxY ;
	float MaxY_poly ;
	// ���|���S���Ƃ̓����蔻��
	if( gbcharainfo[0].FloorNum != 0 ){
		// ���|���S���ɓ����������ǂ����̃t���O��|���Ă���
		gbcharainfo[0].HitFlag = 0 ;
		// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
		MaxY = 0.0f ;
		MaxY_poly = 0.0f ;

		// ���|���S���̐������J��Ԃ�
		for(int i = 0 ; i < gbcharainfo[0].FloorNum ; i ++ ){
			// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
			Poly = Floor[i] ;

			VECTOR cal_pos1 = VAdd( gbcharainfo[0].pos, VGet( 0.0f, PC_HEIGHT, 0.0f ) ) ;
			VECTOR cal_pos2 = VAdd( gbcharainfo[0].pos, VGet( 0.0f, -5.0f, 0.0f ) ) ;
			// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
			LineRes = HitCheck_Line_Triangle( cal_pos1,cal_pos2,Poly->Position[0], Poly->Position[1], Poly->Position[2] ) ;
						
			// �������Ă��Ȃ������牽�����Ȃ�
			if( LineRes.HitFlag == TRUE){
				gbcharainfo[0].PolyCharaHitField[0] = Poly->Position[0] ;
				gbcharainfo[0].PolyCharaHitField[1] = Poly->Position[1] ;
				gbcharainfo[0].PolyCharaHitField[2] = Poly->Position[2] ;
			}
			else{
				continue ;
			}

			// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
			if( gbcharainfo[0].HitFlag == 1 && MaxY > LineRes.Position.y ){
				continue ;
			}

			// �|���S���ɓ��������t���O�𗧂Ă�
			gbcharainfo[0].HitFlag = 1 ;

			// �ڐG�����x���W��ۑ�����
			MaxY = LineRes.Position.y ;
			MaxY_poly = Poly->Position[1].y ;
		}
	}
	// ���|���S���ɓ����������ǂ����ŏ����𕪊�
	if( gbcharainfo[0].HitFlag == 1 ){
		// �ڐG�����|���S���ň�ԍ����x���W���L�����N�^�[�̂x���W�ɂ���
		gbcharainfo[0].pos.y = MaxY ;
		gbcharainfo[0].move.y = 0.0f ;

		if(gbcharainfo[0].mode == JUMPLOOP || gbcharainfo[0].mode == FALL){
			MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].mode = JUMPOUT ;
			gbcharainfo[0].playtime = 0.0f ;
			gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
			gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].move.x = 0.0f ;
			gbcharainfo[0].move.y = 0.0f ;
			gbcharainfo[0].move.z = 0.0f ;
		}
	}
	else{
		// ���ɕ�������� 
		if(gbcharainfo[0].mode != FALL){ // �W�����v��Ԃ���Ȃ�
			MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].mode = FALL ;
			gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
			gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].playtime = 7.0f ;
			MV1SetAttachAnimTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx,gbcharainfo[0].playtime) ;
		}
	}

	// �W�����v����������d�͒ǉ�������
	if(gbcharainfo[0].mode == FALL){
		gbcharainfo[0].move.y -= 0.5f ;
	}

	// ���o�����L�����N�^�[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate( gbcharainfo[0].HitDim ) ;
	return 0;
}

/*------------------------------------------------
|
|
+--------------------------------------------------*/
int Chara_player_Anim( void ){

	// �A�j���[�V�����i�s
	if(charainfo[0].mode != JUMPOUT){
		charainfo[0].playtime += 0.5f ;
	}
	else{
		charainfo[0].playtime += 1.0f ;	
	}

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

	MV1SetAttachAnimTime(charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
	return 0;
}


/*------------------------------------------------
|
|
+--------------------------------------------------*/
int Chara_goblin_Anim( void ){

	// �A�j���[�V�����i�s
	if(gbcharainfo[0].mode != JUMPOUT){
		gbcharainfo[0].playtime += 0.5f ;
	}
	else{
		gbcharainfo[0].playtime += 1.0f ;
	}

	if( gbcharainfo[0].mode != FALL ){
		if(gbcharainfo[0].playtime > gbcharainfo[0].anim_totaltime){
			if(gbcharainfo[0].mode == STAND )
			{
				MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
				gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
				gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			}
			if(gbcharainfo[0].mode == JUMPOUT){
				MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
				gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
				gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
				gbcharainfo[0].mode = STAND ;
			}
			if(gbcharainfo[0].mode == DAMAGE){
				MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
				gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
				gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
				gbcharainfo[0].mode = STAND ;
			}

			gbcharainfo[0].playtime = 0.0f ;

		}
	}
	MV1SetAttachAnimTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx,gbcharainfo[0].playtime) ;
	return 0;
}

