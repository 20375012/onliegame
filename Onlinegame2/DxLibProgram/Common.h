
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Common.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|       全てのファイルで共通のヘッダーファイル
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */
#include <DxLib.h>
#include "Animation.h"

#include "ConsoleWindow.h"	// --- コンソールウィンドウ


#include "math.h"
#include <ctime>        // time
#include <cstdlib>      // srand,rand

/* -----------------------------------------------------------------------------------------
|
|
|       DEFINE宣言
|
|
+ --------------------------------------------------------------------------------------- */
#define CLIENT_WIDTH		512		    // --- クライアント領域のデフォルトの幅, フルスクリーンの場合は水平方向解像度
#define	CLIENT_HEIGHT		480	        // --- クライアント領域のデフォルトの高さ, フルスクリーンの場合は垂直方向解像度

#define SPR1_PASS			"Material/Osero_Back.bmp"
#define SPR2_PASS			"Material/Chip.bmp"
#define SPR3_PASS			"Material/Number.bmp"

#define OSERO_X_MAX			8           // --- オセロの盤面X最大数
#define OSERO_Y_MAX			8           // --- オセロの盤面Y最大数

#define OSERO_CHIP_NON		0			// --- オセロ未使用
#define OSERO_CHIP_BLACK    1			// --- オセロの黒チップ
#define OSERO_CHIP_WHITE    2			// --- オセロの白チップ

#define SERVER_APP			1           // --- サーバーアプリケーションとして起動
#define CLIENT_APP			2           // --- クライアントアプリケーションとして起動

#define	GM_S_WAIT			0			// --- クライアント待ち
#define	GM_S_START			1			// --- スタート
#define	GM_S_PLAY			2			// --- 対戦中
	#define	GM_S_PLAY_TURN		0		// --- 自分の番
	#define	GM_S_PLAY_WAIT		1		// --- 相手の番
#define	GM_S_END			3			// --- 終了

#define	GM_C_FIND			0			// --- サーバー探索
#define	GM_C_START			1			// --- スタート
#define	GM_C_PLAY			2			// --- 対戦中
#define	GM_C_END			3			// --- 終了

#define	KEY_F1				0			// --- F1 キー状態
#define	KEY_MOUSE_L			1			// --- マウス左 キー状態
#define	KEY_MAX				16			// --- キー状態最大値

#define	SPR_MAX				16			// --- 登録スプライト(BMP)最大数

#define SPR_CHIP_CUT_W		25			// --- チップスプライト切り取り幅			
#define SPR_CHIP_CUT_H		25			// --- チップスプライト切り取り高さ

#define CHIP_ADJUST_X		3			// --- チップ配置時の調整値X	
#define CHIP_ADJUST_Y		3			// --- チップ配置時の調整値Y

#define SPR_NUM_CUT_W		32			// --- 数字スプライト切り取り幅			
#define SPR_NUM_CUT_H		48			// --- 数字スプライト切り取り高さ

#define	DISP_BOARD_X		145		// --- 盤の左上からのX座標	
#define	DISP_BOARD_Y		130			// --- 盤の左上からのY座標	

#define PLYER_1P			0			// --- プレイヤー１(サーバー)
#define PLYER_2P			1			// --- プレイヤー２(クライアント)
#define PLYER_MAX			2			// --- プレイヤー最大数

#define SM_EVENT			WM_USER + 1	// --- ソケットメッセージイベント

#define MESSAGE_STR_MAX		256			// --- 最大発言数

#define STAGEFILE_ID								"MAPD"

// ステージデータに含められる最大キャラ数
#define STAGE_CHARA_MAX_NUM							(256)

// ステージデータに含められる最大オブジェクト数
#define STAGE_OBJECT_MAX_NUM						(512)

// ステージデータに含められる最大オブジェクト用モデル数
#define STAGE_OBJECT_MODEL_MAX_NUM					(256)

// ステージデータに含められる最大イベント数
#define STAGE_EVENT_MAX_NUM							(256)

// 一つのイベント辺りに指定できるオブジェクトの最大数
#define STAGE_EVENT_OBJECT_MAX_NUM					(16)

// コリジョンモデルの最大マテリアル数
#define STAGE_COLLISION_MODEL_MATERIAL_MAX_NUM		(32)

// キャラクターのデータファイルが格納されているフォルダ
#define CHARADATA_DIR					"..\\Data\\Character\\"

// キャラクターアニメーションのFPS
#define CHARA_ANIM_FPS					(30)

// キャラクターに付けられるサブ３Ｄモデルの最大数
#define CHARA_MAX_SUBMODEL				(4)

// キャラクターの文字列データで使用できる最大文字数
#define CHARA_MAX_STRINGLENGTH			(64)

// キャラクターの１アニメーションで扱える攻撃判定情報の数
#define CHARA_ATTACK_MAX_NUM			(4)

// キャラクターの攻撃判定用位置情報の最大数
#define CHARA_ATTACK_POS_MAX_NUM		(4)

// アニメーションイベントのサウンドファイルパスで使用できる最大文字数
#define ANIM_EVENT_SOUNDPATH_MAX_LENGTH			(64)

// アニメーションイベントの最大数
#define ANIM_EVENT_MAX_NUM						(64)

// アニメーションのタイムを整数型に変換する際のスケール
#define ANIM_TIME_INT_SCALE						(10000.0f)

// アニメーションで再生する効果音のファイルが格納されているフォルダ
#define ANIM_SE_FILE_DIRECTORY_PATH				"SE\\"

// キャラサイズ
#define PC_WIDTH 80.0f
#define PC_HEIGHT 180.0f


#define EFFECT_SLASH_WIDTH_MAX 15
#define EFFECT_SLASH_WIDTH_MIN 1
#define EFFECT_SLASH_WIDTH_FRAME 3
#define EFFECT_SLASH_WIDTH_SIZE 200
#define EFFECT_SLASH_HEIGHT_SIZE 200

#define EFFECT_SLASH_RADIAN 315

#define CHARA_ENUM_DEFAULT_SIZE		500.0f		// 周囲のポリゴン検出に使用する球の初期サイズ
#define CHARA_MAX_HITCOLL			2048		// 処理するコリジョンポリゴンの最大数

#define CHARA_HIT_HEIGHT			700.0f		// 当たり判定カプセルの高さ

#define SOUND_DIRECTORY_PATH	"..\\Data\\Sound\\"

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
enum CharaMode
{
	STAND,
	RUN,
	JUMPIN,
	JUMPLOOP,
	JUMPOUT,
	ATTACK,
	ATTACK2,
	ATTACK3,
	GDAMAGE,
	GSTAND,
	FALL,
} ;

// キャラクターの当たり判定の情報
typedef struct _SCharaHitInfo
{
	// 当たり判定の幅
	float                 Width;

	// 当たり判定の高さ
	float                 Height;

	// 当たり判定の中心座標
	VECTOR                CenterPosition;
} SCharaHitInfo;

// キャラ情報構造体
typedef struct _CharaInfo{
	int				model1 ;			// キャラのモデルを格納
	int				mode ;				// キャラの状態
	int				attachidx ;			// キャラの現在のアニメーションを格納（アタッチ）
	float			anim_totaltime ;	// アニメのトータルアニメ
	float			playtime ;			// アニメ再生時間
	int				rootflm ;			// アニメのルート
	VECTOR			pos ;				// キャラの座標 x,y,z
	VECTOR			move ;				// キャラの移動量 x,y,z
	int				direction ;			// キャラの向き
	SCharaHitInfo	charahitinfo ;		// キャラヒットチェック用情報

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
	int		osero_board_array[ OSERO_X_MAX * OSERO_Y_MAX ];	// --- マップデータ
	char	message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
	int		non_cnt, black_cnt, white_cnt;					// --- 各チップのカウンタ
} SERVER_SEND_DATA, CLIENT_RECV_DATA;

typedef struct	// --- サーバー受信クライアント送信データ構造体
{
	int		mode;											// --- モード通知			
	POINT	mouse_point;									// --- クライアントポイントデータ
	char	message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
} SERVER_RECV_DATA, CLIENT_SEND_DATA;

/* -----------------------------------------------------------------------------------------
|
|
|       プロトタイプ宣言
|
|
+ --------------------------------------------------------------------------------------- */
LRESULT CALLBACK WindowProc ( HWND, UINT, WPARAM, LPARAM );	// --- ウィンドウプロシージャー

// --- ゲームメイン処理
int Game_Main_Function		( HWND hWnd );					// --- ゲームメイン関数
	// --- サーバー処理
    int Server_Game_Main		( HWND hWnd );				// --- サーバーゲーム処理
		int GM_Server_Wait			( void );				// --- サーバークライアント待ち
		int GM_Server_Start			( void );				// --- サーバー開始時処理
		int GM_Server_Play			( void );				// --- サーバー対戦中時処理
			int GM_Server_Play_1P		( void );			// --- サーバー1P処理
			int GM_Server_Play_2P		( void );			// --- サーバー2P処理
		int GM_Server_End			( HWND hWnd );			// --- サーバー終了処理
		int ServerToClient_Send		( void );				// --- サーバーからクライアントへ送信
		int Server_Disp_Show		( HWND hWnd );			// --- サーバー描画処理

	// --- クライアント処理
    int Client_Game_Main		( HWND hWnd );				// --- クライアントゲーム処理
		int GM_Client_Find			( void );				// --- サーバークライアント待ち
		int GM_Client_Start			( void );				// --- クライアント開始時処理
		int GM_Client_Play			( void );				// --- クライアント対戦中時処理
		int GM_Client_End			( void );				// --- クライアント終了処理
		int ClientToServer_Send		( void );				// --- クライアントからサーバーへ送信
		int Client_Disp_Show		( HWND hWnd );			// --- クライアント描画処理

// --- オセロ連鎖チェック関数
int		Finalize_Check				( void );																				// --- 終了をチェック
int		Pass_Check					( int color1, int color2 );																// --- パス状態をチェック
int		Chip_Set_Check				( POINT dest_point, int color1, int color2 );											// --- チップが置けるかの判定
int		Chip_Chain_Reflexive_Func	( POINT point, int *array, int color1, int color2, int direction, int cnt, int option );// --- 相手のチップを取る処理(再起関数)
int		Chip_Val_Check				( int color );																			// --- 指定のチップを数える

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
extern int					osero_board_array[ OSERO_X_MAX * OSERO_Y_MAX ];
extern int					app_mode;										// --- アプリケーションモード
extern int					game_mode;										// --- ゲームモード
extern int					free_mode;										// --- フリーモード
extern int					play_mode[ PLYER_MAX ];							// --- プレイ中のモード
extern int					chip_val[ PLYER_MAX ];							// --- 各プレイヤーチップ数
extern int					turn_val;										// --- ターン判定番号
extern char					message_str[ MESSAGE_STR_MAX ];					// --- メッセージ文字列
extern int					non_cnt, black_cnt, white_cnt;					// --- 各チップのカウンタ
extern int					key_state[ KEY_MAX ];							// --- キー状態
extern POINT				mouse_point;									// --- マウスカーソル位置
extern HBITMAP				hBitmap;
extern HBITMAP				hBitmapSpr[ SPR_MAX ];
extern HWND					edit_window; 
extern HWND					static_window; 
extern GAMEWINDOW			game_win;

extern int					argc;
extern char					**argv;

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

extern ConsoleWindow g_cWin ;	














