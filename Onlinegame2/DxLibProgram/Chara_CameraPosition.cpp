
/*------------------------------------------------
|
|
+--------------------------------------------------*/
#include "Common.h"

int CameraPosition_Init(void){

	// カメラポジション cpos:カメラ位置　ctgt:カメラ注視点
	if(CHOICESTAGE == 1){
		cpos = VGet(0.0f,1400.0f,-1500.0f) ;
		ctgt = VGet(0.0f,500.0f,-400.0f) ;
	}

	return 0;
}

int CameraPosition(void){

	cpos.x += charainfo[0].move.x ;
	cpos.y += charainfo[0].move.y ;
	cpos.z += charainfo[0].move.z ;

	ctgt.x += charainfo[0].move.x ;
	ctgt.y += charainfo[0].move.y ;
	ctgt.z += charainfo[0].move.z ;

	SetCameraPositionAndTargetAndUpVec(cpos,ctgt,VGet(0.0f,0.0f,1.0f)) ;

	return 0;
}


