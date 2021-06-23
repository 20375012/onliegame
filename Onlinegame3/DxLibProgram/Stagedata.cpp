
/*------------------------------------------------
|
|			
|
+--------------------------------------------------*/
#include "Common.h"

int SetStage_Init( void ){

	stagepos = VGet(0.0f,0.0f,0.0f) ;	//ステージのポジション
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

	return 0 ;
}


