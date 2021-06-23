
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

// --- 必要ファイルのインクルード

#include <windows.h>
#include <stdio.h>	// --- コンソール用
#include "Common.h"
    
HWND	        edit_window; 

/*-----------------------------------------------------------------------------------------------------------*/
int				app_mode;						// --- アプリケーションモード
int				game_mode;						// --- ゲームモード
int				free_mode;						// --- フリーモード

int				play_mode[ PLYER_MAX ];			// --- プレイ中のモード
char			message_str[ MESSAGE_STR_MAX ];	// --- メッセージ文字列
int				key_state[ KEY_MAX ];			// --- キー状態
int				argc;
char 			**argv;

ConsoleWindow	g_cWin;
POINT			mouse_point;					// --- マウスカーソル位置
GAMEWINDOW		game_win =
{
    NULL,
    NULL,
    { CLIENT_WIDTH, CLIENT_HEIGHT },
};
// --- 通信関連データ
SERVER_SEND_DATA	server_send_work;				// --- サーバー送信データ
CLIENT_RECV_DATA	client_recv_work;				// --- クライアント受信データ
SERVER_RECV_DATA	server_recv_work;				// --- サーバー受信データ
CLIENT_SEND_DATA	client_send_work;				// --- クライアント送信データ


WORD                version = MAKEWORD( 2, 0 );     // --- WinSockへの要求バージョン
WSADATA             ws_data;                        // --- WinSockのデータ
SOCKET              sock;							// --- サーバー＆クライアントソケット
struct sockaddr_in  addr;							// --- サーバー＆クライアントアドレス情報
struct sockaddr_in  recv_addr;						// --- レシーブ用
int					addr_len;						// --- レシーブ用アドレスサイズ

/*----------------------------------------------
|
|
-------------------------------------------------------------*/
char test_str[ 256 ];
char String[256] ;

SCharaInfo charainfo[3] ;	// キャラ情報
int anim_nutral,anim_run,anim_jumpin,anim_jumploop,anim_jumpout,anim_attack,anim_attack2,anim_attack3 ;

SCharaInfo gbcharainfo[3] ;	// キャラ情報
int gbanim_nutral,gbanim_damage,gbanim_run ;

int gamemode ;				// 全体的なゲームの管理
int stagedata ;
int stagedata_c ; 

int key ;

int BGMSoundHandle ;
int SESoundHandle ;
int ShadowMapHandle ;	// シャドウマップハンドル

VECTOR stagepos ;	
VECTOR Position ;

MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
MV1_COLL_RESULT_POLY *Poly ;						// ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
HITRESULT_LINE LineRes ;							// 線分とポリゴンとの当たり判定の結果を代入する構造体

/* ===============================================================================
|
|   Game_Main_Function
|
+ ============================================================================= */
int Game_Main_Function( void )
//int Game_Main_Function( HWND hWnd )
{
    switch( app_mode )
    {
        case SERVER_APP: Server_Game_Main( ); break;     // --- サーバゲームメイン処理
        case CLIENT_APP: Client_Game_Main( ); break;     // --- クライアントゲームメイン処理
//        case SERVER_APP: Server_Game_Main( hWnd ); break;     // --- サーバゲームメイン処理
//        case CLIENT_APP: Client_Game_Main( hWnd ); break;     // --- クライアントゲームメイン処理
    }
    return( TRUE );
}

/* ===============================================================================
|
|   Game_END
|
+ ============================================================================= */
int Finalize_Check( void )
{
	int i = 0 ;
	if( i != 0 ){
		return TRUE ;
	}
	return( FALSE );	// --- 継続
}
/* =======================================================================
|
|   Free_Args
|
+ ===================================================================== */
void Free_Args( int argc, char **argv )
{
    int i;

    for( i = 0 ; i < argc ; i++ ) free( argv[ i ] );

    free( argv );
}

/* =======================================================================
|
|   Get_Args
|
+ ===================================================================== */
int Get_Args( const char *cmdline, char ***pargv ) 
{
    int len, i, ct = 0, mct = 0, cch = 0, mcch = 0, dq = 0;
    
	char *tmpb = NULL, *tmpbn, **argv, **argvn;

    len = strlen( cmdline );
    for( i = 0 ; i <= len ; i++ )  
	{
        if( cmdline[ i ] == '\"' )  
		{
            if( dq )
			{
                if( cmdline[ i + 1 ] == ' ' )  dq = 0;
            } 
			else
			{ 
				dq = 1;
			}
        } 
		else if( ( ( !dq ) && ( cmdline[ i ] == ' ' ) ) || ( i == len ) )  
		{
            if( cch||dq||( i ? ( cmdline[ i - 1 ] == '\"' ) : 0 ) )  
			{
                if( ct == mct )  
				{
                    mct += 16;
                    argvn = ( char ** )calloc( mct, sizeof( char * ) );
                    if( !argvn )  
					{
                        free( tmpb );
                        Free_Args( ct, argv );
                        return ( -1 );
                    }
                    if( ct )  
					{
                        memcpy( argvn,argv, ct * sizeof( char * ) );
                        free( argv );
                    }
                    argv = argvn;
                }

                if( !cch )  
				{
                    tmpb = ( char * )malloc( 1 );
                    if( !tmpb)  
					{
                        Free_Args( ct, argv );
                        return ( -1 );
                    }
                    tmpb[ 0 ] = '\0';
                }
                argv[ ct++ ] = tmpb;
                tmpb = NULL;
                cch = 0;
                mcch = 0;
            }
        } 
		else 
		{
            if( cch == mcch )  
			{
                mcch += 64;
                tmpbn = ( char * )malloc( mcch + 1 );
                if( !tmpbn )  
				{
                    free( tmpb );
                    Free_Args( ct, argv );
                    return ( -1 );
                }
                memset( tmpbn, 0, mcch + 1 );
                if( cch )  
				{
                    memcpy( tmpbn, tmpb, cch );
                    free( tmpb );
                }
                tmpb = tmpbn;
            }
            if( dq && ( cmdline[ i ] == '\\' ) && ( cmdline[ i + 1 ] == '\"' ) )  tmpb[ cch ] = cmdline[ i + 1 ];
            else tmpb[ cch ] = cmdline[ i ];
            cch++;
        }
    }

    if( pargv )  *pargv = argv;
    return (  ct ) ;
}

/*------------------------------------------------
|
|
+-------------------------------------------------*/
int Sound_Init( void ){

	int BGMLoopStartPosition = -1 ;
	int BGMLoopEndPosition = -1 ;
	char BGMO_FilePath[13] = "BGM_stg0.ogg" ;
	char SE_FilePath[17] = "dmg_byAxe_00.wav" ;

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


//	PlaySoundMem( BGMSoundHandle, BGMLoopStartPosition >= 0 ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK) ;
//	PlaySoundMem( SESoundHandle, DX_PLAYTYPE_NORMAL) ;

	return 0;
}









