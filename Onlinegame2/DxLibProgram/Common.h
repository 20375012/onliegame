
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
|
|       FILE            : Common.cpp
|       LAST_UP_DATE    : 2012/06/01
|       NAME            : Teacher.K
|
+ ------ Explanation of file --------------------------------------------------------------------------
|       
|       �S�Ẵt�@�C���ŋ��ʂ̃w�b�_�[�t�@�C��
|
+ _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */
#include <DxLib.h>
#include "Animation.h"

#include "ConsoleWindow.h"	// --- �R���\�[���E�B���h�E


#include "math.h"
#include <ctime>        // time
#include <cstdlib>      // srand,rand

/* -----------------------------------------------------------------------------------------
|
|
|       DEFINE�錾
|
|
+ --------------------------------------------------------------------------------------- */
#define CLIENT_WIDTH		512		    // --- �N���C�A���g�̈�̃f�t�H���g�̕�, �t���X�N���[���̏ꍇ�͐��������𑜓x
#define	CLIENT_HEIGHT		480	        // --- �N���C�A���g�̈�̃f�t�H���g�̍���, �t���X�N���[���̏ꍇ�͐��������𑜓x

#define SPR1_PASS			"Material/Osero_Back.bmp"
#define SPR2_PASS			"Material/Chip.bmp"
#define SPR3_PASS			"Material/Number.bmp"

#define OSERO_X_MAX			8           // --- �I�Z���̔Ֆ�X�ő吔
#define OSERO_Y_MAX			8           // --- �I�Z���̔Ֆ�Y�ő吔

#define OSERO_CHIP_NON		0			// --- �I�Z�����g�p
#define OSERO_CHIP_BLACK    1			// --- �I�Z���̍��`�b�v
#define OSERO_CHIP_WHITE    2			// --- �I�Z���̔��`�b�v

#define SERVER_APP			1           // --- �T�[�o�[�A�v���P�[�V�����Ƃ��ċN��
#define CLIENT_APP			2           // --- �N���C�A���g�A�v���P�[�V�����Ƃ��ċN��

#define	GM_S_WAIT			0			// --- �N���C�A���g�҂�
#define	GM_S_START			1			// --- �X�^�[�g
#define	GM_S_PLAY			2			// --- �ΐ풆
	#define	GM_S_PLAY_TURN		0		// --- �����̔�
	#define	GM_S_PLAY_WAIT		1		// --- ����̔�
#define	GM_S_END			3			// --- �I��

#define	GM_C_FIND			0			// --- �T�[�o�[�T��
#define	GM_C_START			1			// --- �X�^�[�g
#define	GM_C_PLAY			2			// --- �ΐ풆
#define	GM_C_END			3			// --- �I��

#define	KEY_F1				0			// --- F1 �L�[���
#define	KEY_MOUSE_L			1			// --- �}�E�X�� �L�[���
#define	KEY_MAX				16			// --- �L�[��ԍő�l

#define	SPR_MAX				16			// --- �o�^�X�v���C�g(BMP)�ő吔

#define SPR_CHIP_CUT_W		25			// --- �`�b�v�X�v���C�g�؂��蕝			
#define SPR_CHIP_CUT_H		25			// --- �`�b�v�X�v���C�g�؂��荂��

#define CHIP_ADJUST_X		3			// --- �`�b�v�z�u���̒����lX	
#define CHIP_ADJUST_Y		3			// --- �`�b�v�z�u���̒����lY

#define SPR_NUM_CUT_W		32			// --- �����X�v���C�g�؂��蕝			
#define SPR_NUM_CUT_H		48			// --- �����X�v���C�g�؂��荂��

#define	DISP_BOARD_X		145		// --- �Ղ̍��ォ���X���W	
#define	DISP_BOARD_Y		130			// --- �Ղ̍��ォ���Y���W	

#define PLYER_1P			0			// --- �v���C���[�P(�T�[�o�[)
#define PLYER_2P			1			// --- �v���C���[�Q(�N���C�A���g)
#define PLYER_MAX			2			// --- �v���C���[�ő吔

#define SM_EVENT			WM_USER + 1	// --- �\�P�b�g���b�Z�[�W�C�x���g

#define MESSAGE_STR_MAX		256			// --- �ő唭����

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
	int		osero_board_array[ OSERO_X_MAX * OSERO_Y_MAX ];	// --- �}�b�v�f�[�^
	char	message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
	int		non_cnt, black_cnt, white_cnt;					// --- �e�`�b�v�̃J�E���^
} SERVER_SEND_DATA, CLIENT_RECV_DATA;

typedef struct	// --- �T�[�o�[��M�N���C�A���g���M�f�[�^�\����
{
	int		mode;											// --- ���[�h�ʒm			
	POINT	mouse_point;									// --- �N���C�A���g�|�C���g�f�[�^
	char	message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
} SERVER_RECV_DATA, CLIENT_SEND_DATA;

/* -----------------------------------------------------------------------------------------
|
|
|       �v���g�^�C�v�錾
|
|
+ --------------------------------------------------------------------------------------- */
LRESULT CALLBACK WindowProc ( HWND, UINT, WPARAM, LPARAM );	// --- �E�B���h�E�v���V�[�W���[

// --- �Q�[�����C������
int Game_Main_Function		( HWND hWnd );					// --- �Q�[�����C���֐�
	// --- �T�[�o�[����
    int Server_Game_Main		( HWND hWnd );				// --- �T�[�o�[�Q�[������
		int GM_Server_Wait			( void );				// --- �T�[�o�[�N���C�A���g�҂�
		int GM_Server_Start			( void );				// --- �T�[�o�[�J�n������
		int GM_Server_Play			( void );				// --- �T�[�o�[�ΐ풆������
			int GM_Server_Play_1P		( void );			// --- �T�[�o�[1P����
			int GM_Server_Play_2P		( void );			// --- �T�[�o�[2P����
		int GM_Server_End			( HWND hWnd );			// --- �T�[�o�[�I������
		int ServerToClient_Send		( void );				// --- �T�[�o�[����N���C�A���g�֑��M
		int Server_Disp_Show		( HWND hWnd );			// --- �T�[�o�[�`�揈��

	// --- �N���C�A���g����
    int Client_Game_Main		( HWND hWnd );				// --- �N���C�A���g�Q�[������
		int GM_Client_Find			( void );				// --- �T�[�o�[�N���C�A���g�҂�
		int GM_Client_Start			( void );				// --- �N���C�A���g�J�n������
		int GM_Client_Play			( void );				// --- �N���C�A���g�ΐ풆������
		int GM_Client_End			( void );				// --- �N���C�A���g�I������
		int ClientToServer_Send		( void );				// --- �N���C�A���g����T�[�o�[�֑��M
		int Client_Disp_Show		( HWND hWnd );			// --- �N���C�A���g�`�揈��

// --- �I�Z���A���`�F�b�N�֐�
int		Finalize_Check				( void );																				// --- �I�����`�F�b�N
int		Pass_Check					( int color1, int color2 );																// --- �p�X��Ԃ��`�F�b�N
int		Chip_Set_Check				( POINT dest_point, int color1, int color2 );											// --- �`�b�v���u���邩�̔���
int		Chip_Chain_Reflexive_Func	( POINT point, int *array, int color1, int color2, int direction, int cnt, int option );// --- ����̃`�b�v����鏈��(�ċN�֐�)
int		Chip_Val_Check				( int color );																			// --- �w��̃`�b�v�𐔂���

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
extern int					osero_board_array[ OSERO_X_MAX * OSERO_Y_MAX ];
extern int					app_mode;										// --- �A�v���P�[�V�������[�h
extern int					game_mode;										// --- �Q�[�����[�h
extern int					free_mode;										// --- �t���[���[�h
extern int					play_mode[ PLYER_MAX ];							// --- �v���C���̃��[�h
extern int					chip_val[ PLYER_MAX ];							// --- �e�v���C���[�`�b�v��
extern int					turn_val;										// --- �^�[������ԍ�
extern char					message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
extern int					non_cnt, black_cnt, white_cnt;					// --- �e�`�b�v�̃J�E���^
extern int					key_state[ KEY_MAX ];							// --- �L�[���
extern POINT				mouse_point;									// --- �}�E�X�J�[�\���ʒu
extern HBITMAP				hBitmap;
extern HBITMAP				hBitmapSpr[ SPR_MAX ];
extern HWND					edit_window; 
extern HWND					static_window; 
extern GAMEWINDOW			game_win;

extern int					argc;
extern char					**argv;

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

extern ConsoleWindow g_cWin ;	














