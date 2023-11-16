#include "pch.h"

int clientId = 0;  // Ŭ���̾�Ʈ id �ʱⰪ 0

std::vector<std::string> ClientNickNname;	// Ŭ���̾�Ʈ�� �г����� �����ϴ� ����
std::vector<PlayerInfo> ClientInfo;

// �Լ��� ����Ͽ� Ŭ���̾�Ʈ ó��
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	struct sockaddr_in clientaddr;
	int addrlen;

	// ������ Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� �г��� ũ�� �ޱ�
	int nick_name_size;
	retval = recv(client_sock, (char*)&nick_name_size, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv(): Client Nickname Size");
		closesocket(client_sock);
		return 0;
	}
	else if (retval == 0)
		return 0;

	// Ŭ���̾�Ʈ�� �г��� �ޱ�
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

	// PlayerInfo �ν��Ͻ��� �����Ͽ� ClientInfo ���Ϳ� �߰�
	PlayerInfo player;
	player.nickname = nick_name;						// Ŭ���̾�Ʈ �г��� ����
	player.portnumber = ntohs(clientaddr.sin_port);		// ��Ʈ��ũ ����Ʈ �������� ȣ��Ʈ ����Ʈ ������ ��ȯ �ʿ�
	player.id = clientId;								// Ŭ���̾�Ʈ �г��� ����

	// Ŭ���̾�Ʈ�� ������ ���Ϳ� ����(�г���, ��Ʈ��ȣ, ���̵�)
	ClientInfo.emplace_back(player);

	// ������ Ŭ���̾�Ʈ ������ �г��� ���
	std::cout << "\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=" << addr << ", ��Ʈ ��ȣ=" << ntohs(clientaddr.sin_port) << ", �г���=" << nick_name << std::endl;

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
				// ������ ���� ����� ���⿡ �ڵ��ϼ��� 
				std::cout << "[" << nick_name << "] " << p->id << "P :";

				switch (p->character)
				{
				case CHARACTER_TYPE::MINJI:
					std::cout << "����" << '\n';
					break;
				case CHARACTER_TYPE::HANNIE:
					std::cout << "�ϴ�" << '\n';
					break;
				case CHARACTER_TYPE::DANIELLE:
					std::cout << "�ٴϿ�" << '\n';
					break;
				case CHARACTER_TYPE::HEARIN:
					std::cout << "����" << '\n';
					break;
				case CHARACTER_TYPE::HYEIN:
					std::cout << "����" << '\n';
					break;
				default:
					break;
				}
			}
			break;

			case int(CS_PACKET_TYPE::CS_INIT_FINISH):
			{
				CS_INIT_FINISH_PACKET* p = reinterpret_cast<CS_INIT_FINISH_PACKET*>(buf);
				std::cout << "[" << nick_name << "] Ŭ���̾�Ʈ �� ����: �ʱ�ȭ �Ϸ� ��ȣ ��Ŷ ����" << '\n';
			}
			break;

			case int(CS_PACKET_TYPE::CS_KEYBOARD_INPUT):
			{
				CS_KEYBOARD_INPUT_PACKET* p = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);
				std::cout << "[" << nick_name << "] Ŭ���̾�Ʈ �� ����: Ű���� �Է� ���� ��Ŷ ����" << '\n';
				
				// ������ Ű���� ������ ���� ���
				std::cout << (int)p->key << std::endl;
				std::cout << (int)p->key_state << std::endl;
			}
			break;

			case int(CS_PACKET_TYPE::CS_SELECT_LOBBY):
			{
				CS_SELECT_LOBBY_PACKET* p = reinterpret_cast<CS_SELECT_LOBBY_PACKET*>(buf);
				std::cout << "[" << nick_name << "] Ŭ���̾�Ʈ �� ����: �κ� ��ư ���� ��Ŷ ����" << '\n';
			}
			break;

			case int(CS_PACKET_TYPE::CS_SELECT_EXIT):
			{
				CS_SELECT_EXIT_PACKET* p = reinterpret_cast<CS_SELECT_EXIT_PACKET*>(buf);
				std::cout << "[" << nick_name << "] Ŭ���̾�Ʈ �� ����: ���� ���� ���� ��Ŷ ����" << '\n';
			}
			break;
		}

		////////////
		// send() //
	    ////////////
	//	SC_PACKET_TYPE type = SC_PACKET_TYPE(rand() % 11 + 1);

	//	// ��Ŷ ���� ������ 
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ID �Ҵ� ���� ��Ŷ ����" << size << '\n';
	//	}
	//	break;

	//	case SC_PACKET_TYPE::SELECT_CHARACTER:
	//	{
	//		SELECT_CHARACTER_PACKET p;

	//		p.type = static_cast<char>(SC_PACKET_TYPE::SELECT_CHARACTER);
	//		size = sizeof(p);

	//		//���⼭ �����ؼ� ������ ��
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ĳ���� ���� ���� ��Ŷ ����" <<size<< '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: �ʱ�ȭ ��û ��ȣ ���� ��Ŷ ����" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ��ȣ ��Ŷ ����" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ��ȣ ��Ŷ ����" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� Ŭ���� ��Ŷ ����" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: �÷��̾� ���� ���� ��Ŷ ����" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ���� ��Ŷ ����" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ���� ��Ŷ ����" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ����ü ���� ���� ��Ŷ ����" << size << '\n';
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
	//	//	std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ������ ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << size << '\n';
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

	//		std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: �÷��̾� ���� ���� ��Ŷ ����" << size << '\n';
	//	}
	//	break;

	//	default:
	//		break;
	//	}

	//	// �����ʹ� 1�ʿ� 60�� ����
	//	Sleep(1000 / 60);	// 60fps
	}

	std::cout << "\n[TCP ����] Ŭ���̾�Ʈ ���� ����: IP �ּ�=" << addr << ", ��Ʈ ��ȣ=" << ntohs(clientaddr.sin_port) << ", �г���=" << nick_name << std::endl;

	closesocket(client_sock);

	return 0;
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

	// ������ ��ſ� ����� ����
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

		clientId++;	// Ŭ���̾�Ʈ id ����

		// Ŭ���̾�Ʈ�� ������ �����忡�� ó��
		HANDLE hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		if (hThread == NULL)
			closesocket(client_sock); // �Լ� ���н� ���� �ݱ�
		else
			CloseHandle(hThread);
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
