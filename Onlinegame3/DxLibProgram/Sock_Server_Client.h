//
///*-----------------------------------------------------------------
//|
//|
//+-------------------------------------------------------------------*/
//
//#define SM_EVENT			WM_USER + 1	// --- �\�P�b�g���b�Z�[�W�C�x���g
//#define MESSAGE_STR_MAX		256			// --- �ő唭����
//
//extern WORD					version;			// --- WinSock�ւ̗v���o�[�W����
//extern WSADATA				ws_data;			// --- WinSock�̃f�[�^
//extern SOCKET				sock;				// --- �T�[�o�[���N���C�A���g�\�P�b�g
//extern struct sockaddr_in	addr;				// --- �T�[�o�[���N���C�A���g�A�h���X���
//extern struct sockaddr_in	recv_addr;			// --- ���V�[�u�p
//extern int					addr_len;			// --- ���V�[�u�p�A�h���X�T�C�Y
//
//// --- �ʐM�f�[�^���
//typedef struct	// --- �T�[�o�[���M�N���C�A���g��M�f�[�^�\����
//{
//	int		mode;											// --- ���[�h�ʒm
//
//	int		chara_mode ;							// �L�����̏��
//	VECTOR	chara_pos ;								// �L�����̍��W x,y,z
//
//
//	char	message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
//
//} SERVER_SEND_DATA, CLIENT_RECV_DATA;
//
//typedef struct	// --- �T�[�o�[��M�N���C�A���g���M�f�[�^�\����
//{
//	int		mode;											// --- ���[�h�ʒm			
//
//	int		chara_mode ;							// �L�����̏��
//	VECTOR	chara_pos ;								// �L�����̍��W x,y,z
//
//	char	message_str[ MESSAGE_STR_MAX ];					// --- ���b�Z�[�W������
//
//} SERVER_RECV_DATA, CLIENT_SEND_DATA;
//
////       �v���g�^�C�v�錾
//int Game_Main_Function		( void );					// --- �Q�[�����C���֐�
//// --- �T�[�o�[����
//int Server_Game_Main			( void );				// --- �T�[�o�[�Q�[������
//	int GM_Server_Wait			( void );				// --- �T�[�o�[�N���C�A���g�҂�
//	int GM_Server_Start			( void );				// --- �T�[�o�[�J�n������
//	int GM_Server_Play			( void );				// --- �T�[�o�[�ΐ풆������
//	int GM_Server_Play_1P		( void );				// --- �T�[�o�[1P����
//	int GM_Server_Play_2P		( void );				// --- �T�[�o�[2P����
//	int GM_Server_End			( void );				// --- �T�[�o�[�I������
//	int ServerToClient_Send		( void );				// --- �T�[�o�[����N���C�A���g�֑��M
//	int Server_Disp_Show		( void );				// --- �T�[�o�[�`�揈��
//
//// --- �N���C�A���g����
//int Client_Game_Main			( void );				// --- �N���C�A���g�Q�[������
//	int GM_Client_Find			( void );				// --- �T�[�o�[�N���C�A���g�҂�
//	int GM_Client_Start			( void );				// --- �N���C�A���g�J�n������
//	int GM_Client_Play			( void );				// --- �N���C�A���g�ΐ풆������
//	int GM_Client_End			( void );				// --- �N���C�A���g�I������
//	int ClientToServer_Send		( void );				// --- �N���C�A���g����T�[�o�[�֑��M
//	int Client_Disp_Show		( void );				// --- �N���C�A���g�`�揈��
//
//int Set(void);
//
//
