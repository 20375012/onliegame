
/*------------------------------------------------
|
|
+--------------------------------------------------*/
#include "Common.h"


int Key_player(void){
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
			charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx);
			charainfo[0].playtime = 0.0f ;
			MV1SetAttachAnimTime(charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
		}

		if (key & PAD_INPUT_A) {
			MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx);
			charainfo[0].mode = ATTACK ;
			charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_attack) ;
			charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx);
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
			charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx);
			charainfo[0].playtime = 0.0f ;
			MV1SetAttachAnimTime(charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
			charainfo[0].move.x = 0.0f ;
			charainfo[0].move.y = 0.0f ;
			charainfo[0].move.z = 0.0f ;
		}
		if (key & PAD_INPUT_C) {
			MV1DetachAnim(charainfo[0].model1,charainfo[0].attachidx);
			charainfo[0].mode = ATTACK3 ;
			charainfo[0].attachidx = MV1AttachAnim(charainfo[0].model1,0,anim_attack3) ;
			charainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(charainfo[0].model1,charainfo[0].attachidx);
			charainfo[0].playtime = 0.0f ;
			MV1SetAttachAnimTime(charainfo[0].model1,charainfo[0].attachidx,charainfo[0].playtime) ;
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
	return 0;
}

int Key_goblin(void){
	// キー操作
	if(gbcharainfo[0].mode == STAND || gbcharainfo[0].mode == RUN ){
		key = GetJoypadInputState(DX_INPUT_KEY_PAD1) ;
		if(key & PAD_INPUT_DOWN){
			gbcharainfo[0].move.x = 0.0f ;
			gbcharainfo[0].move.z = -6.0f ; // 下を押下 手前に移動
			gbcharainfo[0].direction = DOWN ;
		}

		if(key & PAD_INPUT_UP){
			gbcharainfo[0].move.x = 0.0f ;
			gbcharainfo[0].move.z = 6.0f ; // 上を押下 奥に移動
			gbcharainfo[0].direction = UP ;
		}
		if(key & PAD_INPUT_LEFT){
			gbcharainfo[0].move.x = -6.0f ; // 左を押下 左に移動
			gbcharainfo[0].move.z = 0.0f ;
			gbcharainfo[0].direction = LEFT ;
			StopSoundMem( BGMSoundHandle ) ;
		}
		if(key & PAD_INPUT_RIGHT){
			gbcharainfo[0].move.x = 6.0f ; // 右を押下 右に移動
			gbcharainfo[0].move.z = 0.0f ;
			gbcharainfo[0].direction = RIGHT ;
		}

	}
	if(key == 0){
		if(gbcharainfo[0].mode == RUN){
			gbcharainfo[0].move.x = 0.0f ;
			gbcharainfo[0].move.y = 0.0f ;
			gbcharainfo[0].move.z = 0.0f ;
			MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,gbanim_nutral) ;
			gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].mode = STAND ;
		}
	}
	else{
		if(gbcharainfo[0].mode == STAND){
			MV1DetachAnim(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].attachidx = MV1AttachAnim(gbcharainfo[0].model1,0,anim_run) ;
			gbcharainfo[0].anim_totaltime = MV1GetAttachAnimTotalTime(gbcharainfo[0].model1,gbcharainfo[0].attachidx) ;
			gbcharainfo[0].mode = RUN ;
		}
	}
	return 0;
}
