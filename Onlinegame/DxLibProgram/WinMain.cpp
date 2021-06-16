
#include "Common.h"		// --- 全ファイルでの共通ヘッダー

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


	// 全体的なゲームの管理
	int gamemode = eSceneBlank ;

	//ステージのポジション
	VECTOR stagepos = VGet(0.0f,0.0f,0.0f) ;

	// シャドウマップハンドル
	int ShadowMapHandle ;

	VECTOR cpos,ctgt ;
	// カメラポジション cpos:カメラ位置　ctgt:カメラ注視点
	if(CHOICESTAGE == 1){
		cpos = VGet(0.0f,1400.0f,-1500.0f) ;
		ctgt = VGet(0.0f,500.0f,-400.0f) ;
	}
	else{
		cpos = VGet(500.0f,1501.0f,1800.0f) ;
		ctgt = VGet(500.0f,800.0f,2800.0f) ; // 洞窟
	}

	// キャラ情報
	SCharaInfo charainfo[5] ;

	// ステージコリジョン情報
	MV1_COLL_RESULT_POLY_DIM HitDim ;					// キャラクターの周囲にあるポリゴンを検出した結果が代入される当たり判定結果構造体
	int WallNum ;										// 壁ポリゴンと判断されたポリゴンの数
	int FloorNum ;										// 床ポリゴンと判断されたポリゴンの数
	MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列

	int HitFlag ;										// ポリゴンに当たったかどうかを記憶しておくのに使う変数( ０:当たっていない  １:当たった )
	MV1_COLL_RESULT_POLY *Poly ;						// ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
	HITRESULT_LINE LineRes ;							// 線分とポリゴンとの当たり判定の結果を代入する構造体


	// キャラがヒットした床のポリゴン表示の座標
	VECTOR PolyCharaHitField[3] ;

	int key ;

	VECTOR Position ;


	ChangeWindowMode(TRUE) ;

	SetGraphMode(900,600,32) ;

	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;

	// DXライブラリの初期化
	if(DxLib_Init() == -1) return -1 ;

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
	//							キャラ情報セット								//
	// ------------------------------------------------------------------------ //
	if(CHOICESTAGE == 1){
		charainfo[0].pos = VGet(0.0f,500.0f,0.0f) ;		 // 草原
	}
	else{
		charainfo[0].pos = VGet(500.0f,800.0f,2800.0f) ; // 洞窟
	}
	charainfo[0].mode = STAND ;
	charainfo[0].direction = DOWN ;
	charainfo[0].charahitinfo.Height = PC_HEIGHT ;
	charainfo[0].charahitinfo.Width = PC_WIDTH ;
	charainfo[0].charahitinfo.CenterPosition = charainfo[0].pos ;
	charainfo[0].move = VGet(0.0f,0.0f,0.0f) ;

	// --------------------------- // 
	//		キャラモデルセット	   //
	// --------------------------- // 
	charainfo[0].model1 = MV1LoadModel("..\\Player\\PC.mv1") ;
	if(charainfo[0].model1 == -1) return -1 ;
	charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_nutral) ;
	charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx) ;
	// アニメーションして動いてもその場で動いてるような状態
	charainfo[0].rootflm = MV1SearchFrame(charainfo[0].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[0].model1,charainfo[0].rootflm,MGetIdent()) ;
	charainfo[0].playtime = 0.0f ;

	// ------------------------------------------------------------------------ // 
	//							キャラ情報セット								//
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
	//		キャラモデルセット	   //
	// --------------------------- // 
	charainfo[1].model1 = MV1DuplicateModel(charainfo[0].model1) ;
	if(charainfo[1].model1 == -1) return -1 ;
	charainfo[1].attachidx = MV1AttachAnim(charainfo[1].model1,0,anim_nutral) ;
	charainfo[1].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[1].model1,charainfo[1].attachidx) ;
	// アニメーションして動いてもその場で動いてるような状態
	charainfo[1].rootflm = MV1SearchFrame(charainfo[1].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[1].model1,charainfo[1].rootflm,MGetIdent()) ;
	charainfo[1].playtime = 0.0f ;
//--------------------------------------------------------------------------------------------------------------
	charainfo[2].model1 = MV1LoadModel("..\\Goblin\\Goblin.mv1") ;
	if(charainfo[2].model1 == -1) return -1 ;
	charainfo[2].attachidx = MV1AttachAnim(charainfo[2].model1,0,gbanim_nutral) ;
	charainfo[2].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[2].model1,charainfo[2].attachidx) ;
	// アニメーションして動いてもその場で動いてるような状態
	charainfo[2].rootflm = MV1SearchFrame(charainfo[2].model1,"root") ;
	MV1SetFrameUserLocalMatrix(charainfo[2].model1,charainfo[2].rootflm,MGetIdent()) ;
	charainfo[2].playtime = 0.0f ;

	// ステージ情報の読み込み
	if(CHOICESTAGE == 1){
		stagedata = MV1LoadModel("..\\Stage\\Stage00.mv1") ;
		// シャドウマップハンドルの作成


		// ライトの方向を設定
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;

		// シャドウマップが想定するライトの方向もセット
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;

		// シャドウマップに描画する範囲を設定
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;

		stagedata_c = MV1LoadModel("..\\Stage\\Stage00_c.mv1") ;
		if(stagedata == -1) return -1 ;
	}
	else{
		stagedata = MV1LoadModel("..\\Stage\\Stage01.mv1") ;
		// シャドウマップハンドルの作成


		// ライトの方向を設定
		SetLightDirection( VGet( 0.5f, -0.5f, 0.5f ) );
		ShadowMapHandle = MakeShadowMap( 1024, 1024 ) ;

		// シャドウマップが想定するライトの方向もセット
		SetShadowMapLightDirection( ShadowMapHandle, VGet( 0.5f, -0.5f, 0.5f ) ) ;

		// シャドウマップに描画する範囲を設定
		SetShadowMapDrawArea( ShadowMapHandle, VGet( -1000.0f, -1.0f, -1000.0f ), VGet( 1000.0f, 1000.0f, 1000.0f ) ) ;

		stagedata_c = MV1LoadModel("..\\Stage\\Stage01_c.mv1") ;
		if(stagedata == -1) return -1 ;	
	}
	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo( stagedata, -1 ) ;

	SetDrawScreen(DX_SCREEN_BACK) ;

	int MeshNum ;

	// モデルに含まれるメッシュの数を取得する
    MeshNum = MV1GetMeshNum( stagedata ) ;

    // メッシュの数だけループ
    for(int i = 0 ; i < MeshNum ; i ++ ){
	// メッシュに含まれる頂点のローカル座標の最大座標値を描画
		Position = MV1GetMeshMaxPosition( stagedata, i ) ;
		DrawFormatString( 0, 112, GetColor( 255,255,255 ), "Max Position          x %f  y %f  z %f", Position.x, Position.y, Position.z ) ;

		// メッシュに含まれる頂点のローカル座標の最小座標値を描画
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

				// アニメーション進行
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

				// キー操作
				if(charainfo[0].mode == STAND || charainfo[0].mode == RUN ){
					key = GetJoypadInputState(DX_INPUT_KEY_PAD1) ;
					if(key & PAD_INPUT_DOWN){
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.z = -6.0f ; // 下を押下 手前に移動
						charainfo[0].direction = DOWN ;
					}

					if(key & PAD_INPUT_UP){
						charainfo[0].move.x = 0.0f ;
						charainfo[0].move.z = 6.0f ; // 上を押下 奥に移動
						charainfo[0].direction = UP ;
					}
					if(key & PAD_INPUT_LEFT){
						charainfo[0].move.x = -6.0f ; // 左を押下 左に移動
						charainfo[0].move.z = 0.0f ;
						charainfo[0].direction = LEFT ;

						StopSoundMem( BGMSoundHandle ) ;
					}
					if(key & PAD_INPUT_RIGHT){
						charainfo[0].move.x = 6.0f ; // 右を押下 右に移動
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
				// ここに色々処理を追加する
				DrawBox(0,0,900,600,GetColor(255,255,255),true) ; //最後の引数をfalseにすると塗りつぶし無し

				// キャラとヒットチェック
				if(HitCheck_Capsule_Capsule(VAdd(charainfo[0].pos,charainfo[0].move),VAdd(charainfo[0].pos,charainfo[0].move),charainfo[0].charahitinfo.Width / 2,
											VAdd(charainfo[1].pos,charainfo[1].move),VAdd(charainfo[1].pos,charainfo[1].move),charainfo[1].charahitinfo.Width / 2) == TRUE){
					// 移動量を無しに
					charainfo[0].move.x = 0.0f ;
					charainfo[0].move.y = 0.0f ;
					charainfo[0].move.z = 0.0f ;
				}

				if(HitCheck_Capsule_Capsule(VAdd(charainfo[0].pos,charainfo[0].move),VAdd(charainfo[0].pos,charainfo[0].move),charainfo[0].charahitinfo.Width / 2,
											VAdd(charainfo[2].pos,charainfo[2].move),VAdd(charainfo[2].pos,charainfo[2].move),charainfo[2].charahitinfo.Width / 2) == TRUE){
					// 移動量を無しに
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
								// 移動量を無しに

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
								// 移動量を無しに

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
								// 移動量を無しに

							}


					}


			}

				HitDim = MV1CollCheck_Sphere( stagedata, -1, charainfo[0].pos, CHARA_ENUM_DEFAULT_SIZE + VSize( charainfo[0].move ) ) ;
				WallNum = 0 ;
				FloorNum = 0 ;
				// 検出されたポリゴンの数だけ繰り返し
				for(int i = 0 ; i < HitDim.HitNum ; i ++ ){
					// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
					if( HitDim.Dim[i].Normal.y < 0.000001f && HitDim.Dim[i].Normal.y > -0.000001f ){
						printf("壁扱い\n") ;
						// 壁ポリゴンと判断された場合でも、キャラクターのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
						if( HitDim.Dim[i].Position[0].y > charainfo[0].pos.y + 1.0f ||
							HitDim.Dim[i].Position[1].y > charainfo[0].pos.y + 1.0f ||
							HitDim.Dim[i].Position[2].y > charainfo[0].pos.y + 1.0f ){
							// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
							if( WallNum < CHARA_MAX_HITCOLL ){
								// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
								Wall[WallNum] = &HitDim.Dim[i] ;

								// 壁ポリゴンの数を加算する
								WallNum ++ ;
							}
						}
					}
					else{
						// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
						if( FloorNum < CHARA_MAX_HITCOLL ){
							// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
							Floor[FloorNum] = &HitDim.Dim[i] ;

							// 床ポリゴンの数を加算する
							FloorNum ++ ;
						}
					}
				}
				float MaxY ;
				float MaxY_poly ;
				// 床ポリゴンとの当たり判定
				if( FloorNum != 0 ){
					// 床ポリゴンに当たったかどうかのフラグを倒しておく
					HitFlag = 0 ;
					// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
					MaxY = 0.0f ;
					MaxY_poly = 0.0f ;

					// 床ポリゴンの数だけ繰り返し
					for(int i = 0 ; i < FloorNum ; i ++ ){
						// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
						Poly = Floor[i] ;

						VECTOR cal_pos1 = VAdd( charainfo[0].pos, VGet( 0.0f, PC_HEIGHT, 0.0f ) ) ;
						VECTOR cal_pos2 = VAdd( charainfo[0].pos, VGet( 0.0f, -5.0f, 0.0f ) ) ;
						// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
						LineRes = HitCheck_Line_Triangle( cal_pos1,cal_pos2,Poly->Position[0], Poly->Position[1], Poly->Position[2] ) ;
						
						// 当たっていなかったら何もしない
						if( LineRes.HitFlag == TRUE){
							PolyCharaHitField[0] = Poly->Position[0] ;
							PolyCharaHitField[1] = Poly->Position[1] ;
							PolyCharaHitField[2] = Poly->Position[2] ;
						}
						else{
							continue ;
						}

						// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
						if( HitFlag == 1 && MaxY > LineRes.Position.y ){
							continue ;
						}

						// ポリゴンに当たったフラグを立てる
						HitFlag = 1 ;

						// 接触したＹ座標を保存する
						MaxY = LineRes.Position.y ;
						MaxY_poly = Poly->Position[1].y ;
					}
				}
				// 床ポリゴンに当たったかどうかで処理を分岐
				if( HitFlag == 1 ){
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
				MV1CollResultPolyDimTerminate( HitDim ) ;

				// 移動処理
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

				// モデルの回転
				MV1SetRotationXYZ(charainfo[0].model1,VGet(0.0f,1.57f * charainfo[0].direction,0.0f)) ;
				// モデルの移動(配置)
				MV1SetPosition(charainfo[0].model1,charainfo[0].pos);

				// モデルの回転
				MV1SetRotationXYZ(charainfo[1].model1,VGet(0.0f,1.57f * charainfo[1].direction,0.0f)) ;
				// モデルの移動(配置)
				MV1SetPosition(charainfo[1].model1,charainfo[1].pos);

				// モデルの回転
				MV1SetRotationXYZ(charainfo[2].model1,VGet(0.0f,1.57f * charainfo[2].direction,0.0f)) ;
				// モデルの移動(配置)
				MV1SetPosition(charainfo[2].model1,charainfo[2].pos);

				// 地面(配置)
				MV1SetPosition(stagedata,stagepos);

				// シャドウマップへの描画の準備
				ShadowMap_DrawSetup( ShadowMapHandle ) ;

				// モデルの描画 - 影の描画
				for ( int i = 0 ; i < 3 ; i++ )
				{
					MV1DrawModel(charainfo[i].model1) ;
					//MV1DrawModel(charainfo[1].model1) ;
				}
				// 地面描画
//				MV1DrawModel(stagedata) ;				

				// シャドウマップへの描画を終了
				ShadowMap_DrawEnd() ;

				// 描画に使用するシャドウマップを設定
				SetUseShadowMap( 0, ShadowMapHandle ) ;


				// 地面(配置)＆描画
				MV1DrawModel(stagedata) ;

				DrawTriangle3D(PolyCharaHitField[0], PolyCharaHitField[1], PolyCharaHitField[2],
				GetColor( 255,0,0 ), TRUE ) ;

				// モデルの描画
				for ( int i = 0 ; i < 3 ; i++ )
				{
					MV1DrawModel(charainfo[i].model1) ;
					//MV1DrawModel(charainfo[1].model1) ;
				}
				// 描画に使用するシャドウマップの設定を解除
				SetUseShadowMap( 0, -1 ) ;

				ScreenFlip() ;
				break ;
			case eSceneClear :
				break ;
		}

	}


	// シャドウマップの削除
	DeleteShadowMap( ShadowMapHandle ) ;
	// ステージモデルの削除
	MV1DeleteModel( stagedata ) ;
	// キャラクターモデルの削除
	MV1DeleteModel( charainfo[0].model1 ) ;
	MV1DeleteModel( charainfo[1].model1 ) ;

	DxLib_End() ;
	return 0 ;
}



