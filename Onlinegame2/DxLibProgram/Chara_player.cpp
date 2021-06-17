
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

	// アニメーションの読み込み
	anim_nutral = MV1LoadModel("..\\Player\\Anim_Neutral.mv1") ;	// 立ちアニメ
	if(anim_nutral == -1) return -1 ;
	anim_run = MV1LoadModel("..\\Player\\Anim_Run.mv1") ;			// 走りアニメ	
	if(anim_run == -1) return -1 ;
	anim_jumpin = MV1LoadModel("..\\Player\\Anim_Jump_In.mv1") ;		// ジャンプ入り始めアニメ
	if(anim_jumpin == -1) return -1 ;
	anim_jumploop = MV1LoadModel("..\\Player\\Anim_Jump_Loop.mv1") ;	// ジャンプループアニメ
	if(anim_jumploop == -1) return -1 ;
	anim_jumpout = MV1LoadModel("..\\Player\\Anim_Jump_Out.mv1") ;		// ジャンプ着地アニメ
	if(anim_jumpout == -1) return -1 ;
	anim_attack = MV1LoadModel("..\\Player\\Anim_Attack1.mv1") ;		// アタックアニメ
	if(anim_attack == -1) return -1 ;
	anim_attack2 = MV1LoadModel("..\\Player\\Anim_Attack2.mv1") ;		// アタックアニメ2
	if(anim_attack2 == -1) return -1 ;
	anim_attack3 = MV1LoadModel("..\\Player\\Anim_Attack3.mv1") ;		// アタックアニメ2
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
	// 検出されたポリゴンの数だけ繰り返し
	for(int i = 0 ; i < charainfo[0].HitDim.HitNum ; i ++ ){
		// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
		if( charainfo[0].HitDim.Dim[i].Normal.y < 0.000001f && charainfo[0].HitDim.Dim[i].Normal.y > -0.000001f ){
			printf("壁扱い\n") ;
			// 壁ポリゴンと判断された場合でも、キャラクターのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
			if( charainfo[0].HitDim.Dim[i].Position[0].y > charainfo[0].pos.y + 1.0f ||
				charainfo[0].HitDim.Dim[i].Position[1].y > charainfo[0].pos.y + 1.0f ||
				charainfo[0].HitDim.Dim[i].Position[2].y > charainfo[0].pos.y + 1.0f ){
				// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if( charainfo[0].WallNum < CHARA_MAX_HITCOLL ){								
					Wall[charainfo[0].WallNum] = &charainfo[0].HitDim.Dim[i] ;	// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
					charainfo[0].WallNum ++ ;						// 壁ポリゴンの数を加算する
				}
			}
		}
		else{
			// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
			if( charainfo[0].FloorNum < CHARA_MAX_HITCOLL ){							
				Floor[charainfo[0].FloorNum] = &charainfo[0].HitDim.Dim[i] ;		// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
				charainfo[0].FloorNum ++ ;							// 床ポリゴンの数を加算する
			}
		}
	}
	float MaxY ;
	float MaxY_poly ;
	// 床ポリゴンとの当たり判定
	if( charainfo[0].FloorNum != 0 ){
		// 床ポリゴンに当たったかどうかのフラグを倒しておく
		charainfo[0].HitFlag = 0 ;
		// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
		MaxY = 0.0f ;
		MaxY_poly = 0.0f ;

		// 床ポリゴンの数だけ繰り返し
		for(int i = 0 ; i < charainfo[0].FloorNum ; i ++ ){
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
			Poly = Floor[i] ;

			VECTOR cal_pos1 = VAdd( charainfo[0].pos, VGet( 0.0f, PC_HEIGHT, 0.0f ) ) ;
			VECTOR cal_pos2 = VAdd( charainfo[0].pos, VGet( 0.0f, -5.0f, 0.0f ) ) ;
			// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
			LineRes = HitCheck_Line_Triangle( cal_pos1,cal_pos2,Poly->Position[0], Poly->Position[1], Poly->Position[2] ) ;
						
			// 当たっていなかったら何もしない
			if( LineRes.HitFlag == TRUE){
				charainfo[0].PolyCharaHitField[0] = Poly->Position[0] ;
				charainfo[0].PolyCharaHitField[1] = Poly->Position[1] ;
				charainfo[0].PolyCharaHitField[2] = Poly->Position[2] ;
			}
			else{
				continue ;
			}

			// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
			if( charainfo[0].HitFlag == 1 && MaxY > LineRes.Position.y ){
				continue ;
			}

			// ポリゴンに当たったフラグを立てる
			charainfo[0].HitFlag = 1 ;

			// 接触したＹ座標を保存する
			MaxY = LineRes.Position.y ;
			MaxY_poly = Poly->Position[1].y ;
		}
	}
	// 床ポリゴンに当たったかどうかで処理を分岐
	if( charainfo[0].HitFlag == 1 ){
		// 接触したポリゴンで一番高いＹ座標をキャラクターのＹ座標にする
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
		// 宙に浮いた状態 
		if(charainfo[0].mode != FALL){ // ジャンプ状態じゃない
			MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx) ;
			charainfo[0].mode = FALL ;
			charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_jumploop) ;
			charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
			charainfo[0].playtime = 7.0f ;
			MV1SetAttachAnimTime(charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
		}
	}

	// ジャンプ中だったら重力追加させる
	if(charainfo[0].mode == FALL){
		charainfo[0].move.y -= 0.5f ;
	}

	// 検出したキャラクターの周囲のポリゴン情報を開放する
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
	// 検出されたポリゴンの数だけ繰り返し
	for(int i = 0 ; i < gbcharainfo[0].HitDim.HitNum ; i ++ ){
		// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
		if( gbcharainfo[0].HitDim.Dim[i].Normal.y < 0.000001f && gbcharainfo[0].HitDim.Dim[i].Normal.y > -0.000001f ){
			printf("壁扱い\n") ;
			// 壁ポリゴンと判断された場合でも、キャラクターのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
			if( gbcharainfo[0].HitDim.Dim[i].Position[0].y > gbcharainfo[0].pos.y + 1.0f ||
				gbcharainfo[0].HitDim.Dim[i].Position[1].y > gbcharainfo[0].pos.y + 1.0f ||
				gbcharainfo[0].HitDim.Dim[i].Position[2].y > gbcharainfo[0].pos.y + 1.0f ){
				// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if( gbcharainfo[0].WallNum < CHARA_MAX_HITCOLL ){								
					Wall[gbcharainfo[0].WallNum] = &gbcharainfo[0].HitDim.Dim[i] ;	// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
					gbcharainfo[0].WallNum ++ ;						// 壁ポリゴンの数を加算する
				}
			}
		}
		else{
			// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
			if( gbcharainfo[0].FloorNum < CHARA_MAX_HITCOLL ){							
				Floor[gbcharainfo[0].FloorNum] = &gbcharainfo[0].HitDim.Dim[i] ;		// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
				gbcharainfo[0].FloorNum ++ ;							// 床ポリゴンの数を加算する
			}
		}
	}
	float MaxY ;
	float MaxY_poly ;
	// 床ポリゴンとの当たり判定
	if( gbcharainfo[0].FloorNum != 0 ){
		// 床ポリゴンに当たったかどうかのフラグを倒しておく
		gbcharainfo[0].HitFlag = 0 ;
		// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
		MaxY = 0.0f ;
		MaxY_poly = 0.0f ;

		// 床ポリゴンの数だけ繰り返し
		for(int i = 0 ; i < gbcharainfo[0].FloorNum ; i ++ ){
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
			Poly = Floor[i] ;

			VECTOR cal_pos1 = VAdd( gbcharainfo[0].pos, VGet( 0.0f, PC_HEIGHT, 0.0f ) ) ;
			VECTOR cal_pos2 = VAdd( gbcharainfo[0].pos, VGet( 0.0f, -5.0f, 0.0f ) ) ;
			// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
			LineRes = HitCheck_Line_Triangle( cal_pos1,cal_pos2,Poly->Position[0], Poly->Position[1], Poly->Position[2] ) ;
						
			// 当たっていなかったら何もしない
			if( LineRes.HitFlag == TRUE){
				gbcharainfo[0].PolyCharaHitField[0] = Poly->Position[0] ;
				gbcharainfo[0].PolyCharaHitField[1] = Poly->Position[1] ;
				gbcharainfo[0].PolyCharaHitField[2] = Poly->Position[2] ;
			}
			else{
				continue ;
			}

			// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
			if( gbcharainfo[0].HitFlag == 1 && MaxY > LineRes.Position.y ){
				continue ;
			}

			// ポリゴンに当たったフラグを立てる
			gbcharainfo[0].HitFlag = 1 ;

			// 接触したＹ座標を保存する
			MaxY = LineRes.Position.y ;
			MaxY_poly = Poly->Position[1].y ;
		}
	}
	// 床ポリゴンに当たったかどうかで処理を分岐
	if( gbcharainfo[0].HitFlag == 1 ){
		// 接触したポリゴンで一番高いＹ座標をキャラクターのＹ座標にする
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
		// 宙に浮いた状態 
		if(gbcharainfo[0].mode != FALL){ // ジャンプ状態じゃない
			MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].mode = FALL ;
			gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
			gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].playtime = 7.0f ;
			MV1SetAttachAnimTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx,gbcharainfo[0].playtime) ;
		}
	}

	// ジャンプ中だったら重力追加させる
	if(gbcharainfo[0].mode == FALL){
		gbcharainfo[0].move.y -= 0.5f ;
	}

	// 検出したキャラクターの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate( gbcharainfo[0].HitDim ) ;
	return 0;
}

/*------------------------------------------------
|
|
+--------------------------------------------------*/
int Chara_player_Anim( void ){

	// アニメーション進行
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
				charainfo[0].pos.y += charainfo[0].move.y ; //ジャンプ直後の地面めり込みを避けるため
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

	// アニメーション進行
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

