#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <iostream>

#include "../Server/protocol.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ


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

// ���� �Լ� ���� ���
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

	// ����� �μ��� ������ IP �ּҷ� ���
	if (argc > 1) SERVERIP = argv[1];

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	
	// ������ ������ ���
	while (true) {

		// �ְ� �޴� ������ �߿��ϴ�!! ����� Ŭ�󿡼� ���� ������ ���� 
		
		///////////////////////////////////////////////////////////////////////////
		/////////////////////////////// send() ////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		
		char buf[BUFSIZE];
		int size;
		CS_PACKET_TYPE type = CS_PACKET_TYPE(rand() % 5 + 1); // send()Ÿ�� ���� 

		switch (type)
		{
		case CS_PACKET_TYPE::SELECT_CHARACTER:
		{
			SELECT_CHARACTER_PACKET p;
			p.type = static_cast<char>(CS_PACKET_TYPE::SELECT_CHARACTER);
			size = sizeof(p);
			std::cout << "send() - ĳ���� ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
			std::cout << "send() - �ʱ�ȭ �Ϸ� ��ȣ ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
			std::cout << "send() - Ű���� �Է� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
			std::cout << "send() - �κ� ��ư ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
			std::cout << "send() - ���� ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
			std::cout << "�������� ����" << '\n';
			break;
		}
		switch (buf[0]) {
			case int(SC_PACKET_TYPE::SC_MAKE_ID) :
			{
				SC_MAKE_ID_PACKET* p = reinterpret_cast<SC_MAKE_ID_PACKET*>(buf);
				std::cout << "recv() - ID �Ҵ� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
				
			}
				break;
			
			case int(SC_PACKET_TYPE::SELECT_CHARACTER):
			{
				SELECT_CHARACTER_PACKET* p = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
				std::cout << "recv() - ĳ���� ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_INIT):
			{
				SC_INIT_PACKET* p = reinterpret_cast<SC_INIT_PACKET*>(buf);
				std::cout << "recv() - �ʱ�ȭ ��û ��ȣ ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_GAME_START):
			{
				SC_GAME_START_PACKET* p = reinterpret_cast<SC_GAME_START_PACKET*>(buf);
				std::cout << "recv() - ���� ���� ��ȣ ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_GAME_OVER):
			{
				SC_GAME_OVER_PACKET* p = reinterpret_cast<SC_GAME_OVER_PACKET*>(buf);
				std::cout << "recv() - ���� ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_GAME_CLEAR):
			{
				SC_GAME_CLEAR_PACKET* p = reinterpret_cast<SC_GAME_CLEAR_PACKET*>(buf);
				std::cout << "recv() - ���� Ŭ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_PLAYER):
			{
				SC_PLAYER_PACKET* p = reinterpret_cast<SC_PLAYER_PACKET*>(buf);
				std::cout << "recv() - �÷��̾� ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_MONSTER):
			{
				SC_MONSTER_PACKET* p = reinterpret_cast<SC_MONSTER_PACKET*>(buf);
				std::cout << "recv() - ���� ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_BOSS):
			{
				SC_BOSS_PACKET* p = reinterpret_cast<SC_BOSS_PACKET*>(buf);
				std::cout << "recv() - ���� ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_BULLET):
			{
				SC_BULLET_PACKET* p = reinterpret_cast<SC_BULLET_PACKET*>(buf);
				std::cout << "recv() - ����ü ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_ITEM):
			{
				SC_ITEM_PACKET* p = reinterpret_cast<SC_ITEM_PACKET*>(buf);
				std::cout << "recv() - ������ ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			case int(SC_PACKET_TYPE::SC_RANK):
			{
				SC_RANK_PACKET* p = reinterpret_cast<SC_RANK_PACKET*>(buf);
				std::cout << "recv() - �÷��̾� ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
				break;

			default:
				break;
		}
		


		
	}
	
	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
