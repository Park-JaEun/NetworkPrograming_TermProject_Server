#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include "pch.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#define SERVERPORT 9000

std::vector<std::string> ClientNickNname;		// Ŭ���̾�Ʈ�� �г����� �����ϴ� ����

// ���� �Լ� ���� ��� �� ����
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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	// accept()
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return -1;
	}

	// ������ Ŭ���̾�Ʈ ���� ���
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� �г��� ũ�� �ޱ�
	int nick_name_size;
	retval = recv(client_sock, (char*)&nick_name_size, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(client_sock);
		return 1;
	}

	// Ŭ���̾�Ʈ�� �г��� �ޱ�
	char nick_name[256];
	retval = recv(client_sock, nick_name, nick_name_size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(client_sock);
		return 1;
	}
	nick_name[retval] = '\0';  // ������ �г����� ���ڿ���..


	// Ŭ���̾�Ʈ�� �г����� ����
	ClientNickNname.emplace_back(nick_name);

	// ������ Ŭ���̾�Ʈ ������ �г��� ���
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d, �г���=%s\n",
		addr, ntohs(clientaddr.sin_port), nick_name);

	while (1) {
		 ///////////////////////////////////////////////////////////////////////////
		/////////////////////////////// recv() ////////////////////////////////////
	   ///////////////////////////////////////////////////////////////////////////

		char buf[BUFSIZE];
		int size;

		recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		retval = recv(client_sock, buf, size, MSG_WAITALL);
		
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
	 	else if (retval == 0) {
			std::cout << "Ŭ���̾�Ʈ ����" << std::endl;
			break;
		}
		
		// recv() CS_PACKET_TYPE Ÿ�Կ� ���� 
		switch (buf[0]) {
			case int(CS_PACKET_TYPE::SELECT_CHARACTER) :
			{
				SELECT_CHARACTER_PACKET* p = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
				// ������ ���� ����� ���⿡ �ڵ��ϼ��� 
				//p->character = CHARACTER_TYPE::HANNIE;
				std::cout << "Ŭ���̾�Ʈ �� ���� : ĳ���� ���� �Ϸ� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_INIT_FINISH) :
			{
				CS_INIT_FINISH_PACKET* p = reinterpret_cast<CS_INIT_FINISH_PACKET*>(buf);
				std::cout << "Ŭ���̾�Ʈ �� ���� : �ʱ�ȭ �Ϸ� ��ȣ ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_KEYBOARD_INPUT) :
			{
				CS_KEYBOARD_INPUT_PACKET* p = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);
				std::cout << "Ŭ���̾�Ʈ �� ���� : Ű���� �Է� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_SELECT_LOBBY) :
			{
				CS_SELECT_LOBBY_PACKET* p = reinterpret_cast<CS_SELECT_LOBBY_PACKET*>(buf);
				std::cout << "Ŭ���̾�Ʈ �� ���� : �κ� ��ư ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
			break;
			case int(CS_PACKET_TYPE::CS_SELECT_EXIT) :
			{
				CS_SELECT_EXIT_PACKET* p = reinterpret_cast<CS_SELECT_EXIT_PACKET*>(buf);
				std::cout << "Ŭ���̾�Ʈ �� ���� : ���� ���� ���� ��Ŷ�� �޾ҽ��ϴ�" << '\n';
			}
			break;
		}
		//������ ������ õõ�� �������� ���� �ڵ�
		Sleep(2000); 
		///////////////////////////////////////////////////////////////////////////
		/////////////////////////////// send() ////////////////////////////////////
	   ///////////////////////////////////////////////////////////////////////////
		
		SC_PACKET_TYPE type = SC_PACKET_TYPE(rand() % 12 + 1); // send()Ÿ�� ���� 

		switch (type)
		{
			case SC_PACKET_TYPE::SC_MAKE_ID:
			{
				// ��Ŷ ���� ������ 
				SC_MAKE_ID_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ID �Ҵ� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
				// ��Ŷ ���� ������ 
				SELECT_CHARACTER_PACKET p;
				
				p.type = static_cast<char>(SC_PACKET_TYPE::SELECT_CHARACTER);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ĳ���� ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
				
				//���⼭ �����ؼ� ������ ��
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
				// ��Ŷ ���� ������ 
				SC_INIT_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : �ʱ�ȭ ��û ��ȣ ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
			// ���⼭ ���� ����
			case SC_PACKET_TYPE::SC_GAME_START:
			{
				// ��Ŷ ���� ������ 
				SC_GAME_START_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ���� ���� ��ȣ ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
				// ��Ŷ ���� ������ 
				SC_GAME_OVER_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ���� ���� ��ȣ ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
				// ��Ŷ ���� ������ 
				SC_GAME_CLEAR_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ���� Ŭ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
				// ��Ŷ ���� ������ 
				SC_PLAYER_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : �÷��̾� ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
				// ��Ŷ ���� ������ 
				SC_MONSTER_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ���� ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
				// ��Ŷ ���� ������ 
				SC_BOSS_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ���� ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
				// ��Ŷ ���� ������ 
				SC_BULLET_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : ����ü ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
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
			//case SC_PACKET_TYPE::SC_ITEM:
			//{
			//	// ��Ŷ ���� ������ 
			//	SC_ITEM_PACKET p;
			//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
			//	size = sizeof(p);
			//	std::cout << "���� �� Ŭ���̾�Ʈ : ������ ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << '\n';
			//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			//	if (retval == SOCKET_ERROR) {
			//		err_display("send()");
			//		closesocket(client_sock);
			//		WSACleanup();
			//		return 1;
			//	}
			//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			//	if (retval == SOCKET_ERROR) {
			//		err_display("send()");
			//		closesocket(client_sock);
			//		WSACleanup();
			//		return 1;
			//	}
			//}
			//break;

			case SC_PACKET_TYPE::SC_RANK:
			{
				// ��Ŷ ���� ������ 
				SC_RANK_PACKET p;
				p.type = static_cast<char>(SC_PACKET_TYPE::SC_RANK);
				size = sizeof(p);
				std::cout << "���� �� Ŭ���̾�Ʈ : �÷��̾� ���� ���� ��Ŷ�� �����Ͽ����ϴ�." << '\n';
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
		
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d, �г���=%s\n",
		addr, ntohs(clientaddr.sin_port), nick_name);
	// ���� �ݱ�
	closesocket(listen_sock);
	// ���� ����
	WSACleanup();
	return 0;
}
