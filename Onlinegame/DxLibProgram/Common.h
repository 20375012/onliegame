
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	FILE	: Common.h
	NAME	: kz-matsu

+ ------ Explanation of file --------------------------------------------------------------------------
       
	全てのファイルで共通のヘッダーファイル

+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

/* -----------------------------------------------------------------------------------------
|
|       DEFINE宣言
|
+ --------------------------------------------------------------------------------------- */
// ステージデータファイルの先頭に書き込む文字列
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
|       共通ヘッダー
|
+ --------------------------------------------------------------------------------------- */
#include <DxLib.h>
#include "CharaBase.h"
#include "StageData.h"
#include "Animation.h"

#include "ConsoleWindow.h"	// --- コンソールウィンドウ


#include "math.h"
#include <ctime>        // time
#include <cstdlib>      // srand,rand
/* -----------------------------------------------------------------------------------------
|
|       型宣言
|
+ --------------------------------------------------------------------------------------- */

// --- シーンの名前
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

/* -----------------------------------------------------------------------------------------
|
|       構造体宣言
|
+ --------------------------------------------------------------------------------------- */
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

/* -----------------------------------------------------------------------------------------
|
|       プロトタイプ宣言
|
+ --------------------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
|
|       外部参照宣言
|
+ --------------------------------------------------------------------------------------- */
// --- コンソールウィンドウ
extern ConsoleWindow g_cWin ;	
