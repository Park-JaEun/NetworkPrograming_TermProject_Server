#include "pch.h"
#include "define.h"
#include "CTimer.h"

int clientId = 0;  // Ŭ���̾�Ʈ id �ʱⰪ 0

std::vector<PlayerInfo> ClientInfo;
std::array<CPlayer, 3> PlayerArray;

// �Լ��� ����Ͽ� Ŭ���̾�Ʈ ó��
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;	// Ŭ���̾�Ʈ ����
	struct sockaddr_in clientaddr;		// Ŭ���̾�Ʈ �ּ�
	char nick_name[256];				// Ŭ���̾�Ʈ �г���
	char buf[BUFSIZE];					// ��Ŷ ������ ����
	int retval;							// ���ϰ�
	int addrlen;						// �ּ� ����
	int size;							// ��Ŷ ������
	bool isAllReady{ false };			// ��� Ŭ���̾�Ʈ�� �غ� �������� Ȯ��
	bool isAllInit{ false };			// ��� Ŭ���̾�Ʈ�� �ʱ�ȭ �������� Ȯ��

	// ������ Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� �г��� ����
	retval = recv(client_sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(client_sock, nick_name, size, MSG_WAITALL);
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
	player.portnumber = ntohs(clientaddr.sin_port);		// ��Ʈ��ũ ����Ʈ �������� ȣ��Ʈ ����Ʈ ������ ��ȯ
	player.id = clientId;								// Ŭ���̾�Ʈ �г��� ����
	player.isReady = false;								// Ŭ���̾�Ʈ �غ� ���� false�� �ʱ�ȭ
	player.isInit = false;								// Ŭ���̾�Ʈ �ʱ�ȭ ���� false�� �ʱ�ȭ

	// Ŭ���̾�Ʈ�� ������ ���Ϳ� ����(�г���, ��Ʈ��ȣ, ���̵�)
	ClientInfo.emplace_back(player);

	// ������ Ŭ���̾�Ʈ ������ �г��� ���
	std::cout << "\nPlayer " << clientId << " ����: IP �ּ� = " << addr << ", ��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port) << ", �г��� = " << nick_name << std::endl;

	// Ŭ���̾�Ʈ ���� �۽�
	SC_MAKE_ID_PACKET idPacket;
	size = sizeof(SC_MAKE_ID_PACKET);
	idPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	idPacket.id = clientId;

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&idPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "[Player" << clientId << "] ID �Ҵ� ��Ŷ ����" << std::endl;

	// Ŭ���̾�Ʈ ĳ���� ���� ���� ����
	retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(client_sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;

	SELECT_CHARACTER_PACKET* selectPacket = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
	std::cout << "[Player" << clientId << "] ";
	ClientInfo[clientId].isReady = true;	// Ŭ���̾�Ʈ �غ� ���� true�� ����

	switch (selectPacket->character) {
	case CHARACTER_TYPE::MINJI:
		std::cout << "���� ����" << '\n';
		break;

	case CHARACTER_TYPE::HANNIE:
		std::cout << "�ϴ� ����" << '\n';
		break;

	case CHARACTER_TYPE::DANIELLE:
		std::cout << "�ٴϿ� ����" << '\n';
		break;

	case CHARACTER_TYPE::HEARIN:
		std::cout << "���� ����" << '\n';
		break;

	case CHARACTER_TYPE::HYEIN:
		std::cout << "���� ����" << '\n';
		break;

	default:
		break;
	}

	// ��� Ŭ���̾�Ʈ�� �غ� �������� Ȯ�� (����ȭ)
	while (1) {
		isAllReady = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isReady) {
				isAllReady = false;
				break;
			}
		}

		if (isAllReady)
			break;
	}

	// ��� Ŭ���̾�Ʈ�� �غ� ������ ��� �÷��̾�鿡�� �ʱ�ȭ ��ȣ ��Ŷ ����
	if (isAllReady) {
		SC_INIT_PACKET initPacket;
		size = sizeof(SC_INIT_PACKET);
		initPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);

		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(client_sock, reinterpret_cast<char*>(&initPacket), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return 0;
		}

		std::cout << "��� �÷��̾� �غ� �Ϸ�" << std::endl;
		std::cout << "�÷��̾�鿡�� �ʱ�ȭ ��ȣ ��Ŷ ����" << std::endl;
	}

	// �ʱ�ȭ �Ϸ� ��ȣ ��Ŷ ����
	retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(client_sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;

	CS_INIT_FINISH_PACKET* initFinishPacket = reinterpret_cast<CS_INIT_FINISH_PACKET*>(buf);
	std::cout << "[Player" << clientId << "] �������� �ʱ�ȭ �Ϸ�" << std::endl;
	ClientInfo[clientId].isInit = true;	// Ŭ���̾�Ʈ �ʱ�ȭ ���� true�� ����

	// ��� Ŭ���̾�Ʈ�� �ʱ�ȭ�� �Ϸ��ϸ� �÷��̾�鿡�� ���� ���� ��ȣ ��Ŷ ����
	while (1) {
		isAllInit = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isInit) {
				isAllInit = false;
				break;
			}
		}

		if (isAllInit)
			break;
	}

	if (isAllInit) {
		Sleep(3000);	// 3�� ���
		SC_GAME_START_PACKET startPacket;
		size = sizeof(SC_GAME_START_PACKET);
		startPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);

		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(client_sock, reinterpret_cast<char*>(&startPacket), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return 0;
		}

		std::cout << "��� �÷��̾� �ʱ�ȭ �Ϸ�" << std::endl;
		std::cout << "�÷��̾�鿡�� ���� ���� ��ȣ ��Ŷ ����" << std::endl;
	}

	CTimer::GetInst()->init();

	while (1) {

		CTimer::GetInst()->update();

		////////////
		// recv() //
		////////////
		
		// ���� ����
		memset(buf, 0, BUFSIZE);

		// ������ �ޱ�
		retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		retval = recv(client_sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		switch (buf[0]) {
			case int(CS_PACKET_TYPE::CS_KEYBOARD_INPUT):
			{
				CS_KEYBOARD_INPUT_PACKET* p = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);
				std::cout << "[" << nick_name << "] Ŭ���̾�Ʈ �� ����: Ű���� �Է� ���� ��Ŷ ����" << '\n';
				
				// ������ Ű���� ������ ���� ���
				std::cout << (int)p->key << std::endl;
				std::cout << (int)p->key_state << std::endl;

				// �÷��̾� ��ġ �ڱ�͸� ����
				// ĳ���� �浹 ó��, ĳ���Ͷ� �Ѿ�, ĳ���Ͷ� ������
				// ���Ͷ� �浹ó��
				// �� �÷��̾��� ��ġ ������Ʈ�� �� Ŭ���̾�Ʈ�� �����忡�� �迭�� ���������ؼ� ���� �ٲ�
				Vec2 vCurPos = PlayerArray[clientId].GetPos();
				float speed = PlayerArray[clientId].GetSpeed();

				// ��
				if (p->key == KEY::UP && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x, vPos.y - m_fSpeed * DT);*/

					//if (IsInWorld(vDummyPos) && !bIsBoss)
					//	vPos.y -= m_fSpeed * DT;
					//else if (IsInBossRoom(vDummyPos) && bIsBoss)
					//	vPos.y -= m_fSpeed * DT;

					if (IsInWorld(vCurPos))
						vCurPos.y -= speed * DT;
					else if (IsInBossRoom(vCurPos) )
						vCurPos.y -= speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
				}
				if (p->key == KEY::UP && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				// ��
				if (p->key == KEY::DOWN && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x, vPos.y + m_fSpeed * DT);

					if (IsInWorld(vDummyPos) && !bIsBoss)
						vPos.y += m_fSpeed * DT;
					else if (IsInBossRoom(vDummyPos) && bIsBoss)
						vPos.y += m_fSpeed * DT;

					m_eState = PLAYER_STATE::RUN;*/
					if (IsInWorld(vCurPos))
						vCurPos.y += speed * DT;
					else if (IsInBossRoom(vCurPos))
						vCurPos.y += speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
				}
				if (p->key == KEY::DOWN && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				// ��
				if (p->key == KEY::LEFT && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x - m_fSpeed * DT, vPos.y);

					if (IsInWorld(vDummyPos) && !bIsBoss)
						vPos.x -= m_fSpeed * DT;
					else if (IsInBossRoom(vDummyPos) && bIsBoss)
						vPos.x -= m_fSpeed * DT;

					m_eState = PLAYER_STATE::RUN;
					if (m_bDir != DIR_LEFT)
						m_bDir = DIR_LEFT;*/

					if (IsInWorld(vCurPos))
						vCurPos.x -= speed * DT;
					else if (IsInBossRoom(vCurPos))
						vCurPos.x -= speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
					if (PlayerArray[clientId].GetDir() != DIR_LEFT)
						PlayerArray[clientId].SetDir(DIR_LEFT);
				}
				if (p->key == KEY::LEFT && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				// ��
				if (p->key == KEY::RIGHT && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x + m_fSpeed * DT, vPos.y);

					if (IsInWorld(vDummyPos) && !bIsBoss)
						vPos.x += m_fSpeed * DT;
					else if (IsInBossRoom(vDummyPos) && bIsBoss)
						vPos.x += m_fSpeed * DT;

					m_eState = PLAYER_STATE::RUN;
					if (m_bDir != DIR_RIGHT)
						m_bDir = DIR_RIGHT;*/

					if (IsInWorld(vCurPos))
						vCurPos.x += speed * DT;
					else if (IsInBossRoom(vCurPos))
						vCurPos.x += speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
					if (PlayerArray[clientId].GetDir() != DIR_RIGHT)
						PlayerArray[clientId].SetDir(DIR_RIGHT);
				}
				if (p->key == KEY::RIGHT && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				if (p->key == KEY::SPACE && p->key_state == KEY_STATE::TAP) {
					//// �Ѿ� �߻�
					//CreateBullet();

					//// ���� ����Ʈ ���
					//m_EffectAnimator->Play(L"Shooting", false);
					//m_EffectAnimator->FindAnimation(L"Shooting")->SetFrame(0);
				}
		
				PlayerArray[clientId].SetPos(vCurPos);

				// �ٷ� send �÷��̾� ����
				// �÷��̾� ���� 3��¥�� �迭�� ����
				SC_PLAYER_PACKET pp;
				pp.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
				pp.playerPos = PlayerArray[clientId].GetPos();
				pp.playerState = PlayerArray[clientId].GetState();
				pp.playerDir = PlayerArray[clientId].GetDir();
				size = sizeof(pp);

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				retval = send(client_sock, reinterpret_cast<char*>(&pp), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: �÷��̾� ���� ���� ��Ŷ ����" << size << '\n';
			}
			break;

			/*case int(CS_PACKET_TYPE::CS_SELECT_LOBBY):
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
			break;*/
		}

		////////////
		// send() //
	    ////////////
		SC_PACKET_TYPE type = SC_PACKET_TYPE(rand() % 11 + 1);

		// ��Ŷ ���� ������ 
		switch (type) {
		case SC_PACKET_TYPE::SC_MAKE_ID:
		{
			SC_MAKE_ID_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
			size = sizeof(p);
			
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ID �Ҵ� ���� ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SELECT_CHARACTER:
		{
			SELECT_CHARACTER_PACKET p;

			p.type = static_cast<char>(SC_PACKET_TYPE::SELECT_CHARACTER);
			size = sizeof(p);

			//���⼭ �����ؼ� ������ ��
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ĳ���� ���� ���� ��Ŷ ����" <<size<< '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_INIT:
		{
			SC_INIT_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);
			size = sizeof(p);
			
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: �ʱ�ȭ ��û ��ȣ ���� ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_GAME_START:
		{
			SC_GAME_START_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ��ȣ ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_GAME_OVER:
		{
			SC_GAME_OVER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ��ȣ ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_GAME_CLEAR:
		{
			SC_GAME_CLEAR_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� Ŭ���� ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_PLAYER:
		{
			SC_PLAYER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: �÷��̾� ���� ���� ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_MONSTER:
		{
			SC_MONSTER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ���� ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_BOSS:
		{
			SC_BOSS_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ���� ���� ���� ��Ŷ ����" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_BULLET:
		{
			SC_BULLET_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ����ü ���� ���� ��Ŷ ����" << size << '\n';
		}
		break;

		//case SC_PACKET_TYPE::SC_ITEM:
		//{
		//	SC_ITEM_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
		//	size = sizeof(p);
		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}
		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}
		// 
		//	std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: ������ ���� ���� ��Ŷ�� �����Ͽ����ϴ�" << size << '\n';
		//}
		//break;

		case SC_PACKET_TYPE::SC_RANK:
		{
			SC_RANK_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_RANK);
			size = sizeof(p);
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] ���� �� Ŭ���̾�Ʈ: �÷��̾� ���� ���� ��Ŷ ����" << size << '\n';
		}
		break;

		default:
			break;
		}

		// �����ʹ� 1�ʿ� 60�� ����
		//Sleep(1000 / 60);	// 60fps
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

		++clientId;	// Ŭ���̾�Ʈ id ����

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
