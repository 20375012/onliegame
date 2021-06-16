
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	FILE	: Common.h
	NAME	: kz-matsu

+ ------ Explanation of file --------------------------------------------------------------------------
       
	�S�Ẵt�@�C���ŋ��ʂ̃w�b�_�[�t�@�C��

+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */

/* -----------------------------------------------------------------------------------------
|
|       DEFINE�錾
|
+ --------------------------------------------------------------------------------------- */
// �X�e�[�W�f�[�^�t�@�C���̐擪�ɏ������ޕ�����
#define STAGEFILE_ID								"MAPD"

// �X�e�[�W�f�[�^�Ɋ܂߂���ő�L������
#define STAGE_CHARA_MAX_NUM							(256)

// �X�e�[�W�f�[�^�Ɋ܂߂���ő�I�u�W�F�N�g��
#define STAGE_OBJECT_MAX_NUM						(512)

// �X�e�[�W�f�[�^�Ɋ܂߂���ő�I�u�W�F�N�g�p���f����
#define STAGE_OBJECT_MODEL_MAX_NUM					(256)

// �X�e�[�W�f�[�^�Ɋ܂߂���ő�C�x���g��
#define STAGE_EVENT_MAX_NUM							(256)

// ��̃C�x���g�ӂ�Ɏw��ł���I�u�W�F�N�g�̍ő吔
#define STAGE_EVENT_OBJECT_MAX_NUM					(16)

// �R���W�������f���̍ő�}�e���A����
#define STAGE_COLLISION_MODEL_MATERIAL_MAX_NUM		(32)

// �L�����N�^�[�̃f�[�^�t�@�C�����i�[����Ă���t�H���_
#define CHARADATA_DIR					"..\\Data\\Character\\"

// �L�����N�^�[�A�j���[�V������FPS
#define CHARA_ANIM_FPS					(30)

// �L�����N�^�[�ɕt������T�u�R�c���f���̍ő吔
#define CHARA_MAX_SUBMODEL				(4)

// �L�����N�^�[�̕�����f�[�^�Ŏg�p�ł���ő啶����
#define CHARA_MAX_STRINGLENGTH			(64)

// �L�����N�^�[�̂P�A�j���[�V�����ň�����U��������̐�
#define CHARA_ATTACK_MAX_NUM			(4)

// �L�����N�^�[�̍U������p�ʒu���̍ő吔
#define CHARA_ATTACK_POS_MAX_NUM		(4)

// �A�j���[�V�����C�x���g�̃T�E���h�t�@�C���p�X�Ŏg�p�ł���ő啶����
#define ANIM_EVENT_SOUNDPATH_MAX_LENGTH			(64)

// �A�j���[�V�����C�x���g�̍ő吔
#define ANIM_EVENT_MAX_NUM						(64)

// �A�j���[�V�����̃^�C���𐮐��^�ɕϊ�����ۂ̃X�P�[��
#define ANIM_TIME_INT_SCALE						(10000.0f)

// �A�j���[�V�����ōĐ�������ʉ��̃t�@�C�����i�[����Ă���t�H���_
#define ANIM_SE_FILE_DIRECTORY_PATH				"SE\\"

// �L�����T�C�Y
#define PC_WIDTH 80.0f
#define PC_HEIGHT 180.0f


#define EFFECT_SLASH_WIDTH_MAX 15
#define EFFECT_SLASH_WIDTH_MIN 1
#define EFFECT_SLASH_WIDTH_FRAME 3
#define EFFECT_SLASH_WIDTH_SIZE 200
#define EFFECT_SLASH_HEIGHT_SIZE 200

#define EFFECT_SLASH_RADIAN 315

#define CHARA_ENUM_DEFAULT_SIZE		500.0f		// ���͂̃|���S�����o�Ɏg�p���鋅�̏����T�C�Y
#define CHARA_MAX_HITCOLL			2048		// ��������R���W�����|���S���̍ő吔

#define CHARA_HIT_HEIGHT			700.0f		// �����蔻��J�v�Z���̍���

#define SOUND_DIRECTORY_PATH	"..\\Data\\Sound\\"

/* -----------------------------------------------------------------------------------------
|
|       ���ʃw�b�_�[
|
+ --------------------------------------------------------------------------------------- */
#include <DxLib.h>
#include "CharaBase.h"
#include "StageData.h"
#include "Animation.h"

#include "ConsoleWindow.h"	// --- �R���\�[���E�B���h�E


#include "math.h"
#include <ctime>        // time
#include <cstdlib>      // srand,rand
/* -----------------------------------------------------------------------------------------
|
|       �^�錾
|
+ --------------------------------------------------------------------------------------- */

// --- �V�[���̖��O
enum SceneName
{
	eSceneBlank = -1 ,
	eSceneTitle ,
	eScenePlay ,
	eSceneClear ,
	eSceneOver ,
} ;

// --- �L�����̕���
enum Direction
{
	DOWN,
	LEFT,
	UP,
	RIGHT,
} ;

// --- �L�����̏��
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
|       �\���̐錾
|
+ --------------------------------------------------------------------------------------- */
// �L�����N�^�[�̓����蔻��̏��
typedef struct _SCharaHitInfo
{
	// �����蔻��̕�
	float                 Width;

	// �����蔻��̍���
	float                 Height;

	// �����蔻��̒��S���W
	VECTOR                CenterPosition;
} SCharaHitInfo;

// �L�������\����
typedef struct _CharaInfo{
	int				model1 ;			// �L�����̃��f�����i�[
	int				mode ;				// �L�����̏��
	int				attachidx ;			// �L�����̌��݂̃A�j���[�V�������i�[�i�A�^�b�`�j
	float			anim_totaltime ;	// �A�j���̃g�[�^���A�j��
	float			playtime ;			// �A�j���Đ�����
	int				rootflm ;			// �A�j���̃��[�g
	VECTOR			pos ;				// �L�����̍��W x,y,z
	VECTOR			move ;				// �L�����̈ړ��� x,y,z
	int				direction ;			// �L�����̌���
	SCharaHitInfo	charahitinfo ;		// �L�����q�b�g�`�F�b�N�p���

} SCharaInfo ;

/* -----------------------------------------------------------------------------------------
|
|       �v���g�^�C�v�錾
|
+ --------------------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
|
|       �O���Q�Ɛ錾
|
+ --------------------------------------------------------------------------------------- */
// --- �R���\�[���E�B���h�E
extern ConsoleWindow g_cWin ;	
