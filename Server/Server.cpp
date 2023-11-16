#include "pch.h"

int clientId = 0;  // 클라이언트 id 초기값 0

std::vector<std::string> ClientNickNname;	// 클라이언트의 닉네임을 저장하는 벡터
std::vector<PlayerInfo> ClientInfo;

// 함수를 사용하여 클라이언트 처리
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	struct sockaddr_in clientaddr;
	int addrlen;

	// 접속한 클라이언트 정보 출력
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// 클라이언트의 닉네임 크기 받기
	int nick_name_size;
	retval = recv(client_sock, (char*)&nick_name_size, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv(): Client Nickname Size");
		closesocket(client_sock);
		return 0;
	}
	else if (retval == 0)
		return 0;

	// 클라이언트의 닉네임 받기
	char nick_name[256];
	retval = recv(client_sock, nick_name, nick_name_size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv(): Client Nickname");
		closesocket(client_sock);
		return 0;
	}
	else if (retval == 0)
		return 0;
	nick_name[retval] = '\0';

	// PlayerInfo 인스턴스를 생성하여 ClientInfo 벡터에 추가
	PlayerInfo player;
	player.nickname = nick_name;						// 클라이언트 닉네임 저장
	player.portnumber = ntohs(clientaddr.sin_port);		// 네트워크 바이트 순서에서 호스트 바이트 순서로 변환 필요
	player.id = clientId;								// 클라이언트 닉네임 저장

	// 클라이언트의 정보를 벡터에 저장(닉네임, 포트번호, 아이디)
	ClientInfo.emplace_back(player);

	// 접속한 클라이언트 정보와 닉네임 출력
	std::cout << "\n[TCP 서버] 클라이언트 접속: IP 주소=" << addr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << ", 닉네임=" << nick_name << std::endl;

	while (1) {
		////////////
		// recv() //
		////////////

		char buf[BUFSIZE];
		int size;

		retval= recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		retval = recv(client_sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		switch (buf[0]) {
			case int(CS_PACKET_TYPE::SELECT_CHARACTER):
			{
				SELECT_CHARACTER_PACKET* p = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
				// 데이터 정보 변경시 여기에 코딩하세요 
				std::cout << "[" << nick_name << "] " << p->id << "P :";

				switch (p->character)
				{
				case CHARACTER_TYPE::MINJI:
					std::cout << "민지" << '\n';
					break;
				case CHARACTER_TYPE::HANNIE:
					std::cout << "하니" << '\n';
					break;
				case CHARACTER_TYPE::DANIELLE:
					std::cout << "다니엘" << '\n';
					break;
				case CHARACTER_TYPE::HEARIN:
					std::cout << "혜린" << '\n';
					break;
				case CHARACTER_TYPE::HYEIN:
					std::cout << "혜인" << '\n';
					break;
				default:
					break;
				}
			}
			break;

			case int(CS_PACKET_TYPE::CS_INIT_FINISH):
			{
				CS_INIT_FINISH_PACKET* p = reinterpret_cast<CS_INIT_FINISH_PACKET*>(buf);
				std::cout << "[" << nick_name << "] 클라이언트 → 서버: 초기화 완료 신호 패킷 받음" << '\n';
			}
			break;

			case int(CS_PACKET_TYPE::CS_KEYBOARD_INPUT):
			{
				CS_KEYBOARD_INPUT_PACKET* p = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);
				std::cout << "[" << nick_name << "] 클라이언트 → 서버: 키보드 입력 정보 패킷 받음" << '\n';
				
				// 수신한 키보드 정보와 상태 출력
				std::cout << (int)p->key << std::endl;
				std::cout << (int)p->key_state << std::endl;
			}
			break;

			case int(CS_PACKET_TYPE::CS_SELECT_LOBBY):
			{
				CS_SELECT_LOBBY_PACKET* p = reinterpret_cast<CS_SELECT_LOBBY_PACKET*>(buf);
				std::cout << "[" << nick_name << "] 클라이언트 → 서버: 로비 버튼 선택 패킷 받음" << '\n';
			}
			break;

			case int(CS_PACKET_TYPE::CS_SELECT_EXIT):
			{
				CS_SELECT_EXIT_PACKET* p = reinterpret_cast<CS_SELECT_EXIT_PACKET*>(buf);
				std::cout << "[" << nick_name << "] 클라이언트 → 서버: 게임 종료 선택 패킷 받음" << '\n';
			}
			break;
		}

		////////////
		// send() //
	    ////////////
	//	SC_PACKET_TYPE type = SC_PACKET_TYPE(rand() % 11 + 1);

	//	// 패킷 정보 보내기 
	//	switch (type) {
	//	case SC_PACKET_TYPE::SC_MAKE_ID:
	//	{
	//		SC_MAKE_ID_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	//		size = sizeof(p);
	//		
	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}
	//		
	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: ID 할당 관련 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SELECT_CHARACTER:
	//	{
	//		SELECT_CHARACTER_PACKET p;

	//		p.type = static_cast<char>(SC_PACKET_TYPE::SELECT_CHARACTER);
	//		size = sizeof(p);

	//		//여기서 변경해서 보내면 돼
	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}
	//		
	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 캐릭터 선택 관련 패킷 전송" <<size<< '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_INIT:
	//	{
	//		SC_INIT_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);
	//		size = sizeof(p);
	//		
	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}
	//		
	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 초기화 요청 신호 관련 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_GAME_START:
	//	{
	//		SC_GAME_START_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
	//		size = sizeof(p);

	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 시작 신호 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_GAME_OVER:
	//	{
	//		SC_GAME_OVER_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER);
	//		size = sizeof(p);

	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 오버 신호 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_GAME_CLEAR:
	//	{
	//		SC_GAME_CLEAR_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
	//		size = sizeof(p);

	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 클리어 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_PLAYER:
	//	{
	//		SC_PLAYER_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
	//		size = sizeof(p);

	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 플레이어 정보 관련 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_MONSTER:
	//	{
	//		SC_MONSTER_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
	//		size = sizeof(p);

	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 몬스터 정보 관련 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_BOSS:
	//	{
	//		SC_BOSS_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
	//		size = sizeof(p);

	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 보스 정보 관련 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SC_BULLET:
	//	{
	//		SC_BULLET_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
	//		size = sizeof(p);

	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 투사체 정보 관련 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	//case SC_PACKET_TYPE::SC_ITEM:
	//	//{
	//	//	SC_ITEM_PACKET p;
	//	//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
	//	//	size = sizeof(p);
	//	//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//	//	if (retval == SOCKET_ERROR) {
	//	//		err_display("send()");
	//	//		break;
	//	//	}
	//	//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//	//	if (retval == SOCKET_ERROR) {
	//	//		err_display("send()");
	//	//		break;
	//	//	}
	//	// 
	//	//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 아이템 정보 관련 패킷을 전송하였습니다" << size << '\n';
	//	//}
	//	//break;

	//	case SC_PACKET_TYPE::SC_RANK:
	//	{
	//		SC_RANK_PACKET p;
	//		p.type = static_cast<char>(SC_PACKET_TYPE::SC_RANK);
	//		size = sizeof(p);
	//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}
	//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			break;
	//		}

	//		std::cout << "[" << nick_name << "] 서버 → 클라이언트: 플레이어 순위 정보 패킷 전송" << size << '\n';
	//	}
	//	break;

	//	default:
	//		break;
	//	}

	//	// 데이터는 1초에 60번 전송
	//	Sleep(1000 / 60);	// 60fps
	}

	std::cout << "\n[TCP 서버] 클라이언트 접속 종료: IP 주소=" << addr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << ", 닉네임=" << nick_name << std::endl;

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
	if (listen_sock == INVALID_SOCKET) 
		err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) 
		err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) 
		err_quit("listen()");

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

		if (hThread == NULL)
			closesocket(client_sock); // 함수 실패시 소켓 닫기
		else
			CloseHandle(hThread);
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
