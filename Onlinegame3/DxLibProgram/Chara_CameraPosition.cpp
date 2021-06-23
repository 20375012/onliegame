
/*------------------------------------------------
|
|
+--------------------------------------------------*/
#include "Common.h"

int CameraPosition_Init(void){

	// カメラポジション cpos:カメラ位置　ctgt:カメラ注視点
	if(CHOICESTAGE == 1){
		if ( app_mode == SERVER_APP )
		{
			charainfo[0].cpos = VGet(0.0f,1400.0f,-1500.0f) ;
			charainfo[0].ctgt = VGet(0.0f,500.0f,-400.0f) ;
		}

		if ( app_mode == CLIENT_APP )
		{
			gbcharainfo[0].cpos = VGet(750.0f,1400.0f,-1500.0f) ;
			gbcharainfo[0].ctgt = VGet(750.0f,500.0f,-400.0f) ;
		}

	}
	return 0;
}

int CameraPosition(void){

	if ( app_mode == SERVER_APP )
	{
		charainfo[0].cpos.x += charainfo[0].move.x ;
		charainfo[0].cpos.y += charainfo[0].move.y ;
		charainfo[0].cpos.z += charainfo[0].move.z ;

		charainfo[0].ctgt.x += charainfo[0].move.x ;
		charainfo[0].ctgt.y += charainfo[0].move.y ;
		charainfo[0].ctgt.z += charainfo[0].move.z ;

		SetCameraPositionAndTargetAndUpVec(charainfo[0].cpos,charainfo[0].ctgt,VGet(0.0f,0.0f,1.0f)) ;

	}

	if ( app_mode == CLIENT_APP )
	{
		gbcharainfo[0].cpos.x += gbcharainfo[0].move.x ;
		gbcharainfo[0].cpos.y += gbcharainfo[0].move.y ;
		gbcharainfo[0].cpos.z += gbcharainfo[0].move.z ;

		gbcharainfo[0].ctgt.x += gbcharainfo[0].move.x ;
		gbcharainfo[0].ctgt.y += gbcharainfo[0].move.y ;
		gbcharainfo[0].ctgt.z += gbcharainfo[0].move.z ;

		SetCameraPositionAndTargetAndUpVec(gbcharainfo[0].cpos,gbcharainfo[0].ctgt,VGet(0.0f,0.0f,1.0f)) ;

	}


	return 0;
}

