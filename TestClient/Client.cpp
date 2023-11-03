#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <iostream>

#include "../Server/protocol.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크


char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}



int main(int argc, char* argv[])
{
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	
	// 서버와 데이터 통신
	while (true) {

		// 주고 받는 순서가 중요하다!! 현재는 클라에서 먼저 보내고 있음 
		
		///////////////////////////////////////////////////////////////////////////
		/////////////////////////////// send() ////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		
		char buf[BUFSIZE];
		int size;
		CS_PACKET_TYPE type = CS_PACKET_TYPE(rand() % 5 + 1); // send()타입 선택 

		switch (type)
		{
		case CS_PACKET_TYPE::SELECT_CHARACTER:
		{
			SELECT_CHARACTER_PACKET p;
			p.type = static_cast<char>(CS_PACKET_TYPE::SELECT_CHARACTER);
			size = sizeof(p);
			std::cout << "send() - 캐릭터 선택 관련 패킷을 전송하였습니다" << '\n';
			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
			break;
		case CS_PACKET_TYPE::CS_INIT_FINISH:
		{
			CS_INIT_FINISH_PACKET p;
			p.type = static_cast<char>(CS_PACKET_TYPE::CS_INIT_FINISH);
			size = sizeof(p);
			std::cout << "send() - 초기화 완료 신호 패킷을 전송하였습니다" << '\n';
			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
			break;
		case CS_PACKET_TYPE::CS_KEYBOARD_INPUT:
		{
			CS_KEYBOARD_INPUT_PACKET p;
			p.type = static_cast<char>(CS_PACKET_TYPE::CS_KEYBOARD_INPUT);
			size = sizeof(p);
			std::cout << "send() - 키보드 입력 정보 패킷을 전송하였습니다" << '\n';
			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
			break;
		case CS_PACKET_TYPE::CS_SELECT_LOBBY:
		{
			CS_SELECT_LOBBY_PACKET p;
			p.type = static_cast<char>(CS_PACKET_TYPE::CS_SELECT_LOBBY);
			size = sizeof(p);
			std::cout << "send() - 로비 버튼 선택 패킷을 전송하였습니다" << '\n';
			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
			break;
		case CS_PACKET_TYPE::CS_SELECT_EXIT:
		{
			CS_SELECT_EXIT_PACKET p;
			p.type = static_cast<char>(CS_PACKET_TYPE::CS_SELECT_EXIT);
			size = sizeof(p);
			std::cout << "send() - 게임 종료 선택 패킷을 전송하였습니다" << '\n';
			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
			break;
		default:
			break;
		}
		///////////////////////////////////////////////////////////////////////////
		/////////////////////////////// recv() ////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////

		
		int retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		retval = recv(sock, buf, size, MSG_WAITALL);

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			std::cout << "서버연결 종료" << '\n';
			break;
		}
		switch (buf[0]) {
			case int(SC_PACKET_TYPE::SC_MAKE_ID) :
			{
				SC_MAKE_ID_PACKET* p = reinterpret_cast<SC_MAKE_ID_PACKET*>(buf);
				std::cout << "recv() - ID 할당 관련 패킷을 받았습니다" << '\n';
				
			}
				break;
			
			case int(SC_PACKET_TYPE::SELECT_CHARACTER):
			{
				SELECT_CHARACTER_PACKET* p = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
				std::cout << "recv() - 캐릭터 선택 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_INIT):
			{
				SC_INIT_PACKET* p = reinterpret_cast<SC_INIT_PACKET*>(buf);
				std::cout << "recv() - 초기화 요청 신호 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_GAME_START):
			{
				SC_GAME_START_PACKET* p = reinterpret_cast<SC_GAME_START_PACKET*>(buf);
				std::cout << "recv() - 게임 시작 신호 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_GAME_OVER):
			{
				SC_GAME_OVER_PACKET* p = reinterpret_cast<SC_GAME_OVER_PACKET*>(buf);
				std::cout << "recv() - 게임 오버 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_GAME_CLEAR):
			{
				SC_GAME_CLEAR_PACKET* p = reinterpret_cast<SC_GAME_CLEAR_PACKET*>(buf);
				std::cout << "recv() - 게임 클리어 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_PLAYER):
			{
				SC_PLAYER_PACKET* p = reinterpret_cast<SC_PLAYER_PACKET*>(buf);
				std::cout << "recv() - 플레이어 정보 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_MONSTER):
			{
				SC_MONSTER_PACKET* p = reinterpret_cast<SC_MONSTER_PACKET*>(buf);
				std::cout << "recv() - 몬스터 정보 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_BOSS):
			{
				SC_BOSS_PACKET* p = reinterpret_cast<SC_BOSS_PACKET*>(buf);
				std::cout << "recv() - 보스 정보 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_BULLET):
			{
				SC_BULLET_PACKET* p = reinterpret_cast<SC_BULLET_PACKET*>(buf);
				std::cout << "recv() - 투사체 정보 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_ITEM):
			{
				SC_ITEM_PACKET* p = reinterpret_cast<SC_ITEM_PACKET*>(buf);
				std::cout << "recv() - 아이템 정보 관련 패킷을 받았습니다" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_RANK):
			{
				SC_RANK_PACKET* p = reinterpret_cast<SC_RANK_PACKET*>(buf);
				std::cout << "recv() - 플레이어 순위 정보 패킷을 받았습니다" << '\n';
			}
				break;

			default:
				break;
		}
		


		
	}
	
	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
