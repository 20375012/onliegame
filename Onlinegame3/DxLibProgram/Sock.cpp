//
///*------------------------------------------------
//|
//|
//+--------------------------------------------------*/
//#include "WINSOCK2.h"
//#include "windows.h"
//#include "Sock_Server_Client.h"
//
//
//
//WORD                version = MAKEWORD( 2, 0 );     // --- WinSock�ւ̗v���o�[�W����
//WSADATA             ws_data;                        // --- WinSock�̃f�[�^
//SOCKET              sock;							// --- �T�[�o�[���N���C�A���g�\�P�b�g
//struct sockaddr_in  addr;							// --- �T�[�o�[���N���C�A���g�A�h���X���
//struct sockaddr_in  recv_addr;						// --- ���V�[�u�p
//int					addr_len;						// --- ���V�[�u�p�A�h���X�T�C�Y
//
//
//
//int Set(void){
//
//		// --- �A�h���Y�T�C�Y�i�[
//	    addr_len = sizeof( recv_addr );	
//
//		// --- �\�P�b�g�쐬( �{���Ȃ�G���[���������邪�\�[�X�������Ȃ�̂ŏȗ� )
//		WSAStartup( version, &ws_data );
//		sock = socket( AF_INET, SOCK_DGRAM, 0 );
//		if ( app_mode == SERVER_APP ){		// --- �T�[�o�[
//			
//			// --- �\�P�b�g�f�[�^�̏�����
//			addr.sin_family			= AF_INET;
//			addr.sin_port			= htons( ( short )atoi( argv[ 3 ] ) );
//			addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );
//
//			// --- �\�P�b�g�ƃ|�[�g���֘A�t����
//			bind( sock, ( struct sockaddr * )&addr, sizeof( addr ) );
//
//			// --- �\�P�b�g���u���b�L���O�ɂ��A�񓯊��ʒm��v������
//			WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
//		}
//		else if ( app_mode == CLIENT_APP ){	// --- �N���C�A���g
//			// --- �ڑ���ݒ�
//			addr.sin_family			= AF_INET;								// --- �C���^�[�l�b�g
//			addr.sin_port			= htons( ( short )atol( argv[ 3 ] ) );	// --- �ڑ��|�[�g�ԍ��𐔒l�ɂ��Đݒ�
//			addr.sin_addr.s_addr	= inet_addr( argv[ 2 ] );				// --- �ڑ�IP�A�h���X�𐔒l�ɕϊ�
//
//			// --- �\�P�b�g���u���b�L���O�ɂ��A�񓯊��ʒm��v������
//			WSAAsyncSelect( sock, 0, SM_EVENT, FD_READ | FD_WRITE );
//		}
//
//	return 0 ;
//}