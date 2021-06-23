
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Server.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|		サーバーアプリケーション起動時の処理
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- 必要ファイルのインクルード
#include "WINSOCK2.h"
#include "windows.h"
#include "Common.h"      // --- ゲーム共通のヘッダーファイル
/* ===============================================================================
|
|   Server_Game_Main
|       ゲームメイン関数
+ --------------------------------------------------------------------------------
|   IN ---> HWND hWnd           --> ウィンドウハンドル
|   OUT --> int                 --> 終了値
+ ============================================================================= */
int Server_Game_Main( void )
//int Server_Game_Main( HWND hWnd )
{	
	// --- ゲームモード分岐
	switch( game_mode )
	{
		case GM_S_WAIT:		GM_Server_Wait	( );		break;	// --- クライアント待ち
		case GM_S_START:	GM_Server_Start	( );		break;	// --- スタート
		case GM_S_PLAY:		GM_Server_Play	( );		break;	// --- 対戦中
		case GM_S_END:		GM_Server_End	( );		break;	// --- 終了
	}


	ServerToClient_Send	( );		// --- 送信処理
	Server_Disp_Show	( );		//	---	描画
	return( TRUE );
}

/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Wait( void )
{	
	if(	Chara_player_init() == -1)		return -1 ;
	if(	CameraPosition_Init()	== -1)	return -1 ;		// set position_camera
	if(	Chara_goblin_init() == -1)		return -1 ;

	// --- アドレスが格納されていたら
	if ( recv_addr.sin_addr.s_addr != 0 )
	{
		game_mode = GM_S_START;			// --- ゲーム開始
		server_send_work.mode = 1;		// --- ゲーム開始データ格納
	}
	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Start( void )
{	
 
	game_mode = GM_S_PLAY;

	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Play( void )
{	
//	GM_Server_Play_1P( );	// --- サーバー1P処理
//	GM_Server_Play_2P( );	// --- サーバー2P処理

	if( Chara_player_Anim() == -1 ) return -1 ;
	if( Chara_player_fall() == -1) return -1 ;
	if( Chara_goblin_Anim()== -1 ) return -1 ;
	if( Chara_goblin_fall() == -1) return -1 ;

	Key_player();

	for ( int i = 0 ; i < 3 ; i++ )
	{
		charainfo[i].pos.x += charainfo[i].move.x ;
		charainfo[i].pos.y += charainfo[i].move.y ;
		charainfo[i].pos.z += charainfo[i].move.z ;
	}

	if ( Finalize_Check( ) == TRUE )
	{
		server_send_work.mode = 2;		// --- ゲーム終了データ格納
		game_mode = GM_S_END;
	}
	return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Play_1P( void )
{
	return TRUE ;
}
/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_Play_2P( void )
{
	return TRUE ;
}

/* ===============================================================================
|
|
+ ============================================================================= */
int GM_Server_End( void )
{	
    return( TRUE );
}
/* ===============================================================================
|
|
+ ============================================================================= */
int Server_Disp_Show( void )
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
int ServerToClient_Send( void )
{

	memcpy( server_send_work.charainfo, charainfo, sizeof( charainfo[0] ) );
//	server_send_work.stagedata = stagedata ;
//	server_send_work.stagepos = stagepos ;
//	server_send_work.ShadowMapHandle = ShadowMapHandle ;

	// --- データの作成
	memcpy( server_send_work.message_str, message_str, sizeof( message_str ) );		// --- メッセージ文字列
	// --- クライアントに送信
	sendto( sock, ( const char * )&server_send_work, sizeof( SERVER_SEND_DATA ), 0, ( struct sockaddr * )&recv_addr, sizeof( recv_addr ) );	

    return( TRUE );
}


