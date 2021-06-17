
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|		COMMON.H
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */
#include <DxLib.h>
#include <ctime>				// time
#include <cstdlib>				// srand,rand
#include "Animation.h"
#include "ConsoleWindow.h"		// --- コンソールウィンドウ
#include "math.h"


/* -----------------------------------------------------------------------------------------
|
|
|       DEFINE宣言
|
|
+ --------------------------------------------------------------------------------------- */
#define PC_WIDTH			80.0f		// --- キャラサイズ
#define PC_HEIGHT			180.0f		// --- キャラサイズ
#define CLIENT_WIDTH		512		    // --- クライアント領域のデフォルトの幅, フルスクリーンの場合は水平方向解像度
#define	CLIENT_HEIGHT		480	        // --- クライアント領域のデフォルトの高さ, フルスクリーンの場合は垂直方向解像度
#define SERVER_APP			1           // --- サーバーアプリケーションとして起動
#define CLIENT_APP			2           // --- クライアントアプリケーションとして起動

#define	GM_S_WAIT			0			// --- クライアント待ち
#define	GM_S_START			1			// --- スタート
#define	GM_S_PLAY			2			// --- 対戦中
#define	GM_S_PLAY_TURN		0			// --- 自分の番
#define	GM_S_PLAY_WAIT		1			// --- 相手の番
#define	GM_S_END			3			// --- 終了

#define	GM_C_FIND			0			// --- サーバー探索
#define	GM_C_START			1			// --- スタート
#define	GM_C_PLAY			2			// --- 対戦中
#define	GM_C_END			3			// --- 終了

#define	KEY_F1				0			// --- F1 キー状態
#define	KEY_MOUSE_L			1			// --- マウス左 キー状態
#define	KEY_MAX				16			// --- キー状態最大値

#define PLYER_1P			0			// --- プレイヤー１(サーバー)
#define PLYER_2P			1			// --- プレイヤー２(クライアント)
#define PLYER_MAX			2			// --- プレイヤー最大数

#define SM_EVENT			WM_USER + 1	// --- ソケットメッセージイベント
#define MESSAGE_STR_MAX		256			// --- 最大発言数

#define CHOICESTAGE									(1)		//map
#define STAGEFILE_ID								"MAPD"
#define STAGE_CHARA_MAX_NUM							(256)	// ステージデータに含められる最大キャラ数
#define STAGE_OBJECT_MAX_NUM						(512)	// ステージデータに含められる最大オブジェクト数
#define STAGE_OBJECT_MODEL_MAX_NUM					(256)	// ステージデータに含められる最大オブジェクト用モデル数
#define STAGE_EVENT_MAX_NUM							(256)	// ステージデータに含められる最大イベント数
#define STAGE_EVENT_OBJECT_MAX_NUM					(16)	// 一つのイベント辺りに指定できるオブジェクトの最大数
#define STAGE_COLLISION_MODEL_MATERIAL_MAX_NUM		(32)	// コリジョンモデルの最大マテリアル数

#define SOUND_DIRECTORY_PATH						"..\\Data\\Sound\\"

#define CHARADATA_DIR								"..\\Data\\Character\\"// キャラクターのデータファイルが格納されているフォルダ
#define CHARA_ANIM_FPS								(30)	// キャラクターアニメーションのFPS
#define CHARA_MAX_SUBMODEL							(4)		// キャラクターに付けられるサブ３Ｄモデルの最大数
#define CHARA_MAX_STRINGLENGTH						(64)	// キャラクターの文字列データで使用できる最大文字数
#define CHARA_ATTACK_MAX_NUM						(4)		// キャラクターの１アニメーションで扱える攻撃判定情報の数
#define CHARA_ATTACK_POS_MAX_NUM					(4)		// キャラクターの攻撃判定用位置情報の最大数
#define CHARA_ENUM_DEFAULT_SIZE						500.0f		// 周囲のポリゴン検出に使用する球の初期サイズ
#define CHARA_MAX_HITCOLL							2048		// 処理するコリジョンポリゴンの最大数
#define CHARA_HIT_HEIGHT							700.0f		// 当たり判定カプセルの高さ

#define ANIM_EVENT_SOUNDPATH_MAX_LENGTH				(64)	// アニメーションイベントのサウンドファイルパスで使用できる最大文字数
#define ANIM_EVENT_MAX_NUM							(64)	// アニメーションイベントの最大数
#define ANIM_TIME_INT_SCALE							(10000.0f)// アニメーションのタイムを整数型に変換する際のスケール
#define ANIM_SE_FILE_DIRECTORY_PATH					"SE\\"	// アニメーションで再生する効果音のファイルが格納されているフォルダ

/* -----------------------------------------------------------------------------------------
|
|
|       型宣言
|
|
+ --------------------------------------------------------------------------------------- */
enum SceneName
{
	eSceneBlank = -1 ,
	eSceneTitle ,
	eScenePlay ,
	eSceneClear ,
	eSceneOver ,
} ;

// --- キャラの方向
enum Direction
{
	DOWN,
	LEFT,
	UP,
	RIGHT,
} ;

// --- キャラの状態
enum Chara_Player_Mode
{
	STAND,
	RUN,
	JUMPIN,
	JUMPLOOP,
	JUMPOUT,
	ATTACK,
	ATTACK2,
	ATTACK3,
	DAMAGE,
	FALL,
} ;


// キャラクターの当たり判定の情報
typedef struct _SCharaHitInfo
{
	float                 Width;
	float                 Height;
	VECTOR                CenterPosition;
} SCharaHitInfo;

// キャラ情報構造体
typedef struct _CharaInfo{
	int				model1 ;			// キャラのモデルを格納
	int				mode ;				// キャラの状態
	int				attachidx ;			// キャラの現在のアニメーションを格納（アタッチ）
	int				direction ;			// キャラの向き
	float			anim_totaltime ;	// アニメのトータルアニメ
	float			playtime ;			// アニメ再生時間
	int				rootflm ;			// アニメのルート

	MV1_COLL_RESULT_POLY_DIM	HitDim ;				// キャラクターの周囲にあるポリゴンを検出した結果が代入される当たり判定結果構造体
	int WallNum ;										// 壁ポリゴンと判断されたポリゴンの数
	int FloorNum ;										// 床ポリゴンと判断されたポリゴンの数
	int HitFlag ;										// ポリゴンに当たったかどうかを記憶しておくのに使う変数( ０:当たっていない  １:当たった )

	VECTOR			pos ;								// キャラの座標 x,y,z
	VECTOR			move ;								// キャラの移動量 x,y,z
	VECTOR			PolyCharaHitField[3] ;				// キャラがヒットした床のポリゴン表示の座標

	SCharaHitInfo				charahitinfo ;			// キャラヒットチェック用情報
} SCharaInfo ;

// --- ゲームの画面情報
typedef struct
{
    HWND    hWnd;
    HDC     hScreenDC;
    SIZE    win_size;
} GAMEWINDOW;

// --- 通信データ情報
typedef struct	// --- サーバー送信クライアント受信データ構造体
{
	int		mode;											// --- モード通知
	int		chip_val[ PLYER_MAX ];							// --- 各プレイヤーチップ数
	char	message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
	int		non_cnt, black_cnt, white_cnt;					// --- 各チップのカウンタ
} SERVER_SEND_DATA, CLIENT_RECV_DATA;

typedef struct	// --- サーバー受信クライアント送信データ構造体
{
	int		mode;											// --- モード通知			
	POINT	mouse_point;									// --- クライアントポイントデータ
	char	message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
} SERVER_RECV_DATA, CLIENT_SEND_DATA;

//       プロトタイプ宣言
int Game_Main_Function		( HWND hWnd );					// --- ゲームメイン関数
int Server_Game_Main		( HWND hWnd );					// --- サーバーゲーム処理
int Client_Game_Main		( HWND hWnd );					// --- クライアントゲーム処理

int SetServerClient			(void);
int Sound_Init				( void );										// --- 音楽
int Chara_player_init		( void );
int Chara_goblin_init		( void );
int Chara_player_fall		( void );
int Chara_goblin_fall		( void );
int Chara_player_Anim		( void );
int Chara_goblin_Anim		( void );

int CameraPosition_Init		(void);
int CameraPosition			(void);

int Key_player				(void);
int Key_goblin				(void);
// --- コマンドライン引数変換
void	Free_Args	( int argc, char **argv );
int		Get_Args	( const char *cmdline, char ***pargv );
/* -----------------------------------------------------------------------------------------
|
|
|       外部参照宣言
|
|
+ --------------------------------------------------------------------------------------- */
extern int					app_mode;										// --- アプリケーションモード
extern int					game_mode;										// --- ゲームモード
extern int					free_mode;										// --- フリーモード

extern int					play_mode[ PLYER_MAX ];							// --- プレイ中のモード
extern char					message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
extern int					key_state[ KEY_MAX ];							// --- キー状態
extern int					argc;
extern char					**argv;

extern ConsoleWindow		g_cWin;
extern POINT				mouse_point;									// --- マウスカーソル位置
extern GAMEWINDOW			game_win;
// --- 通信関連データ
extern SERVER_SEND_DATA		server_sned_work;								// --- サーバー送信データ
extern CLIENT_RECV_DATA		client_recv_work;								// --- クライアント受信データ
extern SERVER_RECV_DATA		server_recv_work;								// --- サーバー受信データ
extern CLIENT_SEND_DATA		client_sned_work;								// --- クライアント送信データ

extern WORD					version;			// --- WinSockへの要求バージョン
extern WSADATA				ws_data;			// --- WinSockのデータ
extern SOCKET				sock;				// --- サーバー＆クライアントソケット
extern struct sockaddr_in	addr;				// --- サーバー＆クライアントアドレス情報
extern struct sockaddr_in	recv_addr;			// --- レシーブ用
extern int					addr_len;			// --- レシーブ用アドレスサイズ

/*------------------------------------------------
|
|
-----------------------------------------------------------*/
extern	char test_str[ ];
extern	char String[] ;

extern	SCharaInfo charainfo[] ;	// キャラ情報
extern	int anim_nutral,anim_run,anim_jumpin,anim_jumploop,anim_jumpout,anim_attack,anim_attack2,anim_attack3 ;

extern	SCharaInfo gbcharainfo[] ;	// キャラ情報
extern	int gbanim_nutral,gbanim_damage ;

extern	int stagedata ;
extern	int stagedata_c ; 

extern	int key ;

extern	int BGMSoundHandle ;
extern	int SESoundHandle ;
extern	int ShadowMapHandle ;	// シャドウマップハンドル

extern	VECTOR cpos,ctgt ;
extern	VECTOR Position ;

extern	MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
extern	MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
extern	MV1_COLL_RESULT_POLY *Poly ;						// ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
extern	HITRESULT_LINE LineRes ;							// 線分とポリゴンとの当たり判定の結果を代入する構造体




