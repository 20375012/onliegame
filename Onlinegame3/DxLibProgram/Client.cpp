
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Client.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		クライアントアプリケーション起動時の処理
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- 必要ファイルのインクルード
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"      // --- ゲーム共通のヘッダーファイル

/* ===============================================================================
|
|   Client_Game_Main
|       ゲームメイン関数
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|   OUT --> int                 --> 終了値
+ ============================================================================= */
int Client_Game_Main( void )
//int Client_Game_Main( HWND hWnd )
{
	// --- ゲームモード分岐
	switch( game_mode )
	{
		case GM_C_FIND:		GM_Client_Find	( );	break;	// --- サーバー探索
		case GM_C_START:	GM_Client_Start	( );	break;	// --- スタート
		case GM_C_PLAY:		GM_Client_Play	( );	break;	// --- 対戦中
		case GM_C_END:		GM_Client_End	( );	break;	// --- 終了
	}
	// --- クライアント常時処理	
	ClientToServer_Send	( );		// --- 送信処理
	Client_Disp_Show	( );	// --- 描画処理
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

	// --- サーバーに送信後、開始を待つ
	if ( free_mode == 0 )
	{
		client_send_work.mode = 1;	// --- サーバーに通達
		free_mode++;
	}
	else
	{
		// --- サーバーから開始要請が到着していたら
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
		MV1SetRotationXYZ(charainfo[i].model1,VGet(0.0f,1.57f * charainfo[0].direction,0.0f)) ;					// モデルの回転
		MV1SetPosition(charainfo[i].model1,charainfo[i].pos);													// モデルの移動(配置)
		MV1SetRotationXYZ(gbcharainfo[i].model1,VGet(0.0f,1.57f * gbcharainfo[0].direction,0.0f)) ;					// モデルの回転
		MV1SetPosition(gbcharainfo[i].model1,gbcharainfo[i].pos);													// モデルの移動(配置)
	}
	MV1SetPosition(stagedata,stagepos);						// 地面(配置)

	ShadowMap_DrawSetup( ShadowMapHandle ) ;				// シャドウマップへの描画の準備
	for ( int i = 0 ; i < 3 ; i++ )							// モデルの描画 - 影の描画
	{
		MV1DrawModel(charainfo[i].model1) ;
		MV1DrawModel(gbcharainfo[i].model1) ;
	}
	ShadowMap_DrawEnd() ;									// シャドウマップへの描画を終了
	SetUseShadowMap( 0, ShadowMapHandle ) ;					// 描画に使用するシャドウマップを設定

	MV1DrawModel(stagedata) ;								// 地面(配置)＆描画

	DrawTriangle3D(charainfo[0].PolyCharaHitField[0], charainfo[0].PolyCharaHitField[1], charainfo[0].PolyCharaHitField[2],GetColor( 255,0,0 ), TRUE ) ;
	DrawTriangle3D(gbcharainfo[0].PolyCharaHitField[0], gbcharainfo[0].PolyCharaHitField[1], gbcharainfo[0].PolyCharaHitField[2],GetColor( 255,255,0 ), TRUE ) ;
	for ( int i = 0 ; i < 3 ; i++ )							// モデルの描画
	{
		MV1DrawModel(charainfo[i].model1) ;
		MV1DrawModel(gbcharainfo[i].model1) ;
	}
				
	SetUseShadowMap( 0, -1 ) ;								// 描画に使用するシャドウマップの設定を解除
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
	
	// --- データの作成
	memcpy( client_send_work.message_str, message_str, sizeof( message_str ) );		// --- メッセージ文字列
	// --- サーバーに送信
	sendto( sock, ( const char * )&client_send_work, sizeof( CLIENT_SEND_DATA ), 0, ( struct sockaddr * )&addr, sizeof( addr ) );	
	
    return( TRUE );    return( TRUE );
}




