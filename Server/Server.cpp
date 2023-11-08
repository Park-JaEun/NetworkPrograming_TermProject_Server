#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <iostream>
#include "protocol.h"
#include "Global.h"
#include <vector>
#include <mutex>
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000

std::vector<std::string> ClientNickNname;		// 클라이언트의 닉네임을 저장하는 벡터


int clientId = 0;  // 클라이언트 id 초기값 0

struct PlayerInfo {

	std::string nickname;
	int portnumber;
	int id;
};

std::vector<PlayerInfo> ClientInfo;


// 소켓 함수 오류 출력 후 종료
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

// 함수를 사용하여 클라이언트 처리
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	struct sockaddr_in clientaddr;
	int addrlen;
	long long file_size;
	char buf[BUFSIZE];

	//////

	// 접속한 클라이언트 정보 출력
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// 클라이언트의 닉네임 크기 받기
	int nick_name_size;
	retval = recv(client_sock, (char*)&nick_name_size, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv(1)");
		closesocket(client_sock);
		return 1;
	}

	// 클라이언트의 닉네임 받기
	char nick_name[256];
	retval = recv(client_sock, nick_name, nick_name_size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv(2)");
		closesocket(client_sock);
		return 1;
	}
	nick_name[retval] = '\0';  // 수신한 닉네임을 문자열로..


	// clientaddr.sin_port를 정의해야 하는데, 이 부분은 별도의 네트워크 코드와 연관이 있습니다.

	// PlayerInfo 인스턴스를 생성하여 ClientInfo 벡터에 추가
	PlayerInfo player;
	player.nickname = nick_name;		// 클라이언트 닉네임 저장
	player.portnumber = ntohs(clientaddr.sin_port);  // 네트워크 바이트 순서에서 호스트 바이트 순서로 변환 필요
	player.id = clientId;				// 클라이언트 닉네임 저장


	// 클라이언트의 정보를 벡터에 저장(닉네임, 포트번호, 아이디)
	ClientInfo.emplace_back(player);

	// 접속한 클라이언트 정보와 닉네임 출력
	std::cout << "\n[TCP 서버] 클라이언트 접속: IP 주소=" << addr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << ", 닉네임=" << nick_name << std::endl;



	//////
	while (1) {
		///////////////////////////////////////////////////////////////////////////
	   /////////////////////////////// recv() ////////////////////////////////////
	  ///////////////////////////////////////////////////////////////////////////

		char buf[BUFSIZE];
		int size;

		recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(client_sock);
			WSACleanup();
			return 1;
		}
		retval = recv(client_sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(client_sock);
			WSACleanup();
			return 1;
		}

		// recv() CS_PACKET_TYPE 타입에 따라 
		switch (buf[0]) {
			case int(CS_PACKET_TYPE::SELECT_CHARACTER) :
			{
				SELECT_CHARACTER_PACKET* p = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
				// 데이터 정보 변경시 여기에 코딩하세요 
				//p->character = CHARACTER_TYPE::HANNIE;
				std::cout << "[" << nick_name << "]클라이언트 → 서버 : 캐릭터 선택 완료 패킷을 받았습니다" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_INIT_FINISH) :
			{
				CS_INIT_FINISH_PACKET* p = reinterpret_cast<CS_INIT_FINISH_PACKET*>(buf);
				std::cout << "[" << nick_name << "]클라이언트 → 서버 : 초기화 완료 신호 패킷을 받았습니다" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_KEYBOARD_INPUT) :
			{
				CS_KEYBOARD_INPUT_PACKET* p = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);
				std::cout << "[" << nick_name << "]클라이언트 → 서버 : 키보드 입력 정보 패킷을 받았습니다" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_SELECT_LOBBY) :
			{
				CS_SELECT_LOBBY_PACKET* p = reinterpret_cast<CS_SELECT_LOBBY_PACKET*>(buf);
				std::cout << "[" << nick_name << "]클라이언트 → 서버 : 로비 버튼 선택 패킷을 받았습니다" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_SELECT_EXIT) :
			{
				CS_SELECT_EXIT_PACKET* p = reinterpret_cast<CS_SELECT_EXIT_PACKET*>(buf);
				std::cout << "[" << nick_name << "]클라이언트 → 서버 : 게임 종료 선택 패킷을 받았습니다" << '\n';
			}
			break;
		}
		//데이터 전송을 천천히 보기위해 넣은 코드
		Sleep(2000);
		///////////////////////////////////////////////////////////////////////////
		/////////////////////////////// send() ////////////////////////////////////
	   ///////////////////////////////////////////////////////////////////////////

		SC_PACKET_TYPE type = SC_PACKET_TYPE(rand() % 12 + 1); // send()타입 선택 

		switch (type)
		{
		case SC_PACKET_TYPE::SC_MAKE_ID:
		{
			// 패킷 정보 보내기 
			SC_MAKE_ID_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : ID 할당 관련 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;

		case SC_PACKET_TYPE::SELECT_CHARACTER:
		{
			// 패킷 정보 보내기 
			SELECT_CHARACTER_PACKET p;

			p.type = static_cast<char>(SC_PACKET_TYPE::SELECT_CHARACTER);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 캐릭터 선택 관련 패킷을 전송하였습니다" <<size<< '\n';

			//여기서 변경해서 보내면 돼
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_INIT:
		{
			// 패킷 정보 보내기 
			SC_INIT_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 초기화 요청 신호 관련 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		// 여기서 부터 수정
		case SC_PACKET_TYPE::SC_GAME_START:
		{
			// 패킷 정보 보내기 
			SC_GAME_START_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 게임 시작 신호 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_GAME_OVER:
		{
			// 패킷 정보 보내기 
			SC_GAME_OVER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 게임 오버 신호 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_GAME_CLEAR:
		{
			// 패킷 정보 보내기 
			SC_GAME_CLEAR_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 게임 클리어 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_PLAYER:
		{
			// 패킷 정보 보내기 
			SC_PLAYER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 플레이어 정보 관련 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_MONSTER:
		{
			// 패킷 정보 보내기 
			SC_MONSTER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 몬스터 정보 관련 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_BOSS:
		{
			// 패킷 정보 보내기 
			SC_BOSS_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 보스 정보 관련 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_BULLET:
		{
			// 패킷 정보 보내기 
			SC_BULLET_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 투사체 정보 관련 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;
		case SC_PACKET_TYPE::SC_ITEM:
		{
			// 패킷 정보 보내기 
			SC_ITEM_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 아이템 정보 관련 패킷을 전송하였습니다" << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;

		case SC_PACKET_TYPE::SC_RANK:
		{
			// 패킷 정보 보내기 
			SC_RANK_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_RANK);
			size = sizeof(p);
			std::cout << "[" << nick_name << "]서버 → 클라이언트 : 플레이어 순위 정보 패킷을 전송하였습니다." << size << '\n';
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(client_sock);
				WSACleanup();
				return 1;
			}
		}
		break;

		default:
			break;
		}

	}

	std::cout << "\n[TCP 서버] 클라이언트 접속 종료: IP 주소=" << addr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << ", 닉네임=" << nick_name << std::endl;

	//////

	closesocket(client_sock);

	return 0;
}


int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	while (1) {

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		clientId++;	// 클라이언트 id 설정

		// 클라이언트를 별도의 스레드에서 처리
		HANDLE hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }      // 함수 실패시 소켓 닫기
		else { CloseHandle(hThread); }

	}

	// 소켓 닫기
	closesocket(listen_sock);
	// 윈속 종료
	WSACleanup();
	return 0;
}
