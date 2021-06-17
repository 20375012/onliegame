
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|		COMMON.H
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */
#include <DxLib.h>
#include <ctime>				// time
#include <cstdlib>				// srand,rand
#include "Animation.h"
#include "ConsoleWindow.h"		// --- �R���\�[���E�B���h�E
#include "math.h"


/* -----------------------------------------------------------------------------------------
|
|
|       DEFINE�錾
|
|
+ --------------------------------------------------------------------------------------- */
#define PC_WIDTH			80.0f		// --- �L�����T�C�Y
#define PC_HEIGHT			180.0f		// --- �L�����T�C�Y
#define CLIENT_WIDTH		512		    // --- �N���C�A���g�̈�̃f�t�H���g�̕�, �t���X�N���[���̏ꍇ�͐��������𑜓x
#define	CLIENT_HEIGHT		480	        // --- �N���C�A���g�̈�̃f�t�H���g�̍���, �t���X�N���[���̏ꍇ�͐��������𑜓x
#define SERVER_APP			1           // --- �T�[�o�[�A�v���P�[�V�����Ƃ��ċN��
#define CLIENT_APP			2           // --- �N���C�A���g�A�v���P�[�V�����Ƃ��ċN��

#define	GM_S_WAIT			0			// --- �N���C�A���g�҂�
#define	GM_S_START			1			// --- �X�^�[�g
#define	GM_S_PLAY			2			// --- �ΐ풆
#define	GM_S_PLAY_TURN		0			// --- �����̔�
#define	GM_S_PLAY_WAIT		1			// --- ����̔�
#define	GM_S_END			3			// --- �I��

#define	GM_C_FIND			0			// --- �T�[�o�[�T��
#define	GM_C_START			1			// --- �X�^�[�g
#define	GM_C_PLAY			2			// --- �ΐ풆
#define	GM_C_END			3			// --- �I��

#define	KEY_F1				0			// --- F1 �L�[���
#define	KEY_MOUSE_L			1			// --- �}�E�X�� �L�[���
#define	KEY_MAX				16			// --- �L�[��ԍő�l

#define PLYER_1P			0			// --- �v���C���[�P(�T�[�o�[)
#define PLYER_2P			1			// --- �v���C���[�Q(�N���C�A���g)
#define PLYER_MAX			2			// --- �v���C���[�ő吔

#define SM_EVENT			WM_USER + 1	// --- �\�P�b�g���b�Z�[�W�C�x���g
#define MESSAGE_STR_MAX		256			// --- �ő唭����

#define CHOICESTAGE									(1)		//map
#define STAGEFILE_ID								"MAPD"
#define STAGE_CHARA_MAX_NUM							(256)	// �X�e�[�W�f�[�^�Ɋ܂߂���ő�L������
#define STAGE_OBJECT_MAX_NUM						(512)	// �X�e�[�W�f�[�^�Ɋ܂߂���ő�I�u�W�F�N�g��
#define STAGE_OBJECT_MODEL_MAX_NUM					(256)	// �X�e�[�W�f�[�^�Ɋ܂߂���ő�I�u�W�F�N�g�p���f����
#define STAGE_EVENT_MAX_NUM							(256)	// �X�e�[�W�f�[�^�Ɋ܂߂���ő�C�x���g��
#define STAGE_EVENT_OBJECT_MAX_NUM					(16)	// ��̃C�x���g�ӂ�Ɏw��ł���I�u�W�F�N�g�̍ő吔
#define STAGE_COLLISION_MODEL_MATERIAL_MAX_NUM		(32)	// �R���W�������f���̍ő�}�e���A����

#define SOUND_DIRECTORY_PATH						"..\\Data\\Sound\\"

#define CHARADATA_DIR								"..\\Data\\Character\\"// �L�����N�^�[�̃f�[�^�t�@�C�����i�[����Ă���t�H���_
#define CHARA_ANIM_FPS								(30)	// �L�����N�^�[�A�j���[�V������FPS
#define CHARA_MAX_SUBMODEL							(4)		// �L�����N�^�[�ɕt������T�u�R�c���f���̍ő吔
#define CHARA_MAX_STRINGLENGTH						(64)	// �L�����N�^�[�̕�����f�[�^�Ŏg�p�ł���ő啶����
#define CHARA_ATTACK_MAX_NUM						(4)		// �L�����N�^�[�̂P�A�j���[�V�����ň�����U��������̐�
#define CHARA_ATTACK_POS_MAX_NUM					(4)		// �L�����N�^�[�̍U������p�ʒu���̍ő吔
#define CHARA_ENUM_DEFAULT_SIZE						500.0f		// ���͂̃|���S�����o�Ɏg�p���鋅�̏����T�C�Y
#define CHARA_MAX_HITCOLL							2048		// ��������R���W�����|���S���̍ő吔
#define CHARA_HIT_HEIGHT							700.0f		// �����蔻��J�v�Z���̍���

#define ANIM_EVENT_SOUNDPATH_MAX_LENGTH				(64)	// �A�j���[�V�����C�x���g�̃T�E���h�t�@�C���p�X�Ŏg�p�ł���ő啶����
#define ANIM_EVENT_MAX_NUM							(64)	// �A�j���[�V�����C�x���g�̍ő吔
#define ANIM_TIME_INT_SCALE							(10000.0f)// �A�j���[�V�����̃^�C���𐮐��^�ɕϊ�����ۂ̃X�P�[��
#define ANIM_SE_FILE_DIRECTORY_PATH					"SE\\"	// �A�j���[�V�����ōĐ�������ʉ��̃t�@�C�����i�[����Ă���t�H���_

/* -----------------------------------------------------------------------------------------
|
|
|       �^�錾
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

// --- �L�����̕���
enum Direction
{
	DOWN,
	LEFT,
	UP,
	RIGHT,
} ;

// --- �L�����̏��
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


// �L�����N�^�[�̓����蔻��̏��
typedef struct _SCharaHitInfo
{
	float                 Width;
	float                 Height;
	VECTOR                CenterPosition;
} SCharaHitInfo;

// �L�������\����
typedef struct _CharaInfo{
	int				model1 ;			// �L�����̃��f�����i�[
	int				mode ;				// �L�����̏��
	int				attachidx ;			// �L�����̌��݂̃A�j���[�V�������i�[�i�A�^�b�`�j
	int				direction ;			// �L�����̌���
	float			anim_totaltime ;	// �A�j���̃g�[�^���A�j��
	float			playtime ;			// �A�j���Đ�����
	int				rootflm ;			// �A�j���̃��[�g

	MV1_COLL_RESULT_POLY_DIM	HitDim ;				// �L�����N�^�[�̎��͂ɂ���|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	int WallNum ;										// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int FloorNum ;										// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	int HitFlag ;										// �|���S���ɓ����������ǂ������L�����Ă����̂Ɏg���ϐ�( �O:�������Ă��Ȃ�  �P:�������� )

	VECTOR			pos ;								// �L�����̍��W x,y,z
	VECTOR			move ;								// �L�����̈ړ��� x,y,z
	VECTOR			PolyCharaHitField[3] ;				// �L�������q�b�g�������̃|���S���\���̍��W

	SCharaHitInfo				charahitinfo ;			// �L�����q�b�g�`�F�b�N�p���
} SCharaInfo ;

// --- �Q�[���̉�ʏ��
typedef struct
{
    HWND    hWnd;
    HDC     hScreenDC;
    SIZE    win_size;
} GAMEWINDOW;

// --- �ʐM�f�[�^���
typedef struct	// --- �T�[�o�[���M�N���C�A���g��M�f�[�^�\����
{
	int		mode;											// --- ���[�h�ʒm
	int		chip_val[ PLYER_MAX ];							// --- �e�v���C���[�`�b�v��
	char	message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
	int		non_cnt, black_cnt, white_cnt;					// --- �e�`�b�v�̃J�E���^
} SERVER_SEND_DATA, CLIENT_RECV_DATA;

typedef struct	// --- �T�[�o�[��M�N���C�A���g���M�f�[�^�\����
{
	int		mode;											// --- ���[�h�ʒm			
	POINT	mouse_point;									// --- �N���C�A���g�|�C���g�f�[�^
	char	message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
} SERVER_RECV_DATA, CLIENT_SEND_DATA;

//       �v���g�^�C�v�錾
int Game_Main_Function		( HWND hWnd );					// --- �Q�[�����C���֐�
int Server_Game_Main		( HWND hWnd );					// --- �T�[�o�[�Q�[������
int Client_Game_Main		( HWND hWnd );					// --- �N���C�A���g�Q�[������

int SetServerClient			(void);
int Sound_Init				( void );										// --- ���y
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
// --- �R�}���h���C�������ϊ�
void	Free_Args	( int argc, char **argv );
int		Get_Args	( const char *cmdline, char ***pargv );
/* -----------------------------------------------------------------------------------------
|
|
|       �O���Q�Ɛ錾
|
|
+ --------------------------------------------------------------------------------------- */
extern int					app_mode;										// --- �A�v���P�[�V�������[�h
extern int					game_mode;										// --- �Q�[�����[�h
extern int					free_mode;										// --- �t���[���[�h

extern int					play_mode[ PLYER_MAX ];							// --- �v���C���̃��[�h
extern char					message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
extern int					key_state[ KEY_MAX ];							// --- �L�[���
extern int					argc;
extern char					**argv;

extern ConsoleWindow		g_cWin;
extern POINT				mouse_point;									// --- �}�E�X�J�[�\���ʒu
extern GAMEWINDOW			game_win;
// --- �ʐM�֘A�f�[�^
extern SERVER_SEND_DATA		server_sned_work;								// --- �T�[�o�[���M�f�[�^
extern CLIENT_RECV_DATA		client_recv_work;								// --- �N���C�A���g��M�f�[�^
extern SERVER_RECV_DATA		server_recv_work;								// --- �T�[�o�[��M�f�[�^
extern CLIENT_SEND_DATA		client_sned_work;								// --- �N���C�A���g���M�f�[�^

extern WORD					version;			// --- WinSock�ւ̗v���o�[�W����
extern WSADATA				ws_data;			// --- WinSock�̃f�[�^
extern SOCKET				sock;				// --- �T�[�o�[���N���C�A���g�\�P�b�g
extern struct sockaddr_in	addr;				// --- �T�[�o�[���N���C�A���g�A�h���X���
extern struct sockaddr_in	recv_addr;			// --- ���V�[�u�p
extern int					addr_len;			// --- ���V�[�u�p�A�h���X�T�C�Y

/*------------------------------------------------
|
|
-----------------------------------------------------------*/
extern	char test_str[ ];
extern	char String[] ;

extern	SCharaInfo charainfo[] ;	// �L�������
extern	int anim_nutral,anim_run,anim_jumpin,anim_jumploop,anim_jumpout,anim_attack,anim_attack2,anim_attack3 ;

extern	SCharaInfo gbcharainfo[] ;	// �L�������
extern	int gbanim_nutral,gbanim_damage ;

extern	int stagedata ;
extern	int stagedata_c ; 

extern	int key ;

extern	int BGMSoundHandle ;
extern	int SESoundHandle ;
extern	int ShadowMapHandle ;	// �V���h�E�}�b�v�n���h��

extern	VECTOR cpos,ctgt ;
extern	VECTOR Position ;

extern	MV1_COLL_RESULT_POLY *Wall[CHARA_MAX_HITCOLL] ;		// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
extern	MV1_COLL_RESULT_POLY *Floor[CHARA_MAX_HITCOLL] ;	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
extern	MV1_COLL_RESULT_POLY *Poly ;						// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
extern	HITRESULT_LINE LineRes ;							// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����




