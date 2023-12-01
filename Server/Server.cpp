#include "pch.h"
#include "CTimer.h"
#include "CObjectMgr.h"
#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CEventMgr.h"
#include "CBullet.h"

std::mutex g_mutex;
int clientId = 0;		// Ŭ���̾�Ʈ id �ʱⰪ 0

std::vector<PlayerInfo> ClientInfo;					// Ŭ���̾�Ʈ ���� ����

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
	CObject* pCharacter = new CPlayer;

	// ������ Ŭ���̾�Ʈ ���� ����
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
	{
		std::lock_guard<std::mutex> lock{ g_mutex };		// ���ؽ� ���
		player.nickname = nick_name;						// Ŭ���̾�Ʈ �г��� ����
		player.portnumber = ntohs(clientaddr.sin_port);		// ��Ʈ��ũ ����Ʈ �������� ȣ��Ʈ ����Ʈ ������ ��ȯ
		player.id = clientId++;								// Ŭ���̾�Ʈ ID �Ҵ�
		player.isReady = false;								// Ŭ���̾�Ʈ �غ� ���� false�� �ʱ�ȭ
		player.isInit = false;								// Ŭ���̾�Ʈ �ʱ�ȭ ���� false�� �ʱ�ȭ

		// Ŭ���̾�Ʈ�� ������ ���Ϳ� ����(�г���, ��Ʈ��ȣ, ���̵�)
		ClientInfo.emplace_back(player);

		// ������ Ŭ���̾�Ʈ ������ �г��� ���
		std::cout << "\nPlayer " << player.id << " ����: IP �ּ� = " << addr << ", ��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port) << ", �г��� = " << nick_name << std::endl;
	}

	// Ŭ���̾�Ʈ ���� �۽�
	SC_MAKE_ID_PACKET idPacket;
	idPacket.type	= static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	idPacket.id		= player.id;
	size = sizeof(SC_MAKE_ID_PACKET);

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&idPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "[Player" << idPacket.id << "] ID �Ҵ� ��Ŷ ����" << std::endl;

	// Ŭ���̾�Ʈ ĳ���� ���� ���� ����
	retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(client_sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;

	SELECT_CHARACTER_PACKET* pSelectPacket = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
	{
		std::lock_guard<std::mutex> lock{ g_mutex };
		((CPlayer*)pCharacter)->SetType(pSelectPacket->character);
		((CPlayer*)pCharacter)->SetName(L"Player" + std::to_wstring(player.id));
		((CPlayer*)pCharacter)->SetPos(Vec2(0.f, 0.f));
		((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
		((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
		((CPlayer*)pCharacter)->CreateCollider();
		((CPlayer*)pCharacter)->GetCollider()->SetScale(Vec2(31.f, 30.f));

		ClientInfo[player.id].isReady = true;	// Ŭ���̾�Ʈ �غ� ���� true�� ����
		CObjectMgr::GetInst()->AddObject(pCharacter, GROUP_TYPE::PLAYER);	// �÷��̾� ��ü ���
	}

	switch (pSelectPacket->character) {
	case CHARACTER_TYPE::MINJI:
		std::cout << "[Player" << player.id << "] " << "���� ����" << std::endl;
		break;

	case CHARACTER_TYPE::HANNIE:
		std::cout << "[Player" << player.id << "] " << "�ϴ� ����" << std::endl;
		break;

	case CHARACTER_TYPE::DANIELLE:
		std::cout << "[Player" << player.id << "] " << "�ٴϿ� ����" << std::endl;
		break;

	case CHARACTER_TYPE::HEARIN:
		std::cout << "[Player" << player.id << "] " << "���� ����" << std::endl;
		break;

	case CHARACTER_TYPE::HYEIN:
		std::cout << "[Player" << player.id << "] " << "���� ����" << std::endl;
		break;

	default:
		break;
	}

	// Ŭ���̾�Ʈ�� 3�� �����ְ�, ��� �غ� ������ ��� �÷��̾�鿡�� �ʱ�ȭ ��ȣ ��Ŷ ����
	while (1) {
		std::lock_guard<std::mutex> lock{ g_mutex };	// ���ؽ� ���
		isAllReady = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isReady) {
				isAllReady = false;
				break;
			}
		}

		if (isAllReady && ClientInfo.size() == MAX_PLAYER)	// ��� Ŭ���̾�Ʈ�� �غ� �����̸� ���� Ż��
			break;
	}

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

	// ������ ������ �ٸ� Ŭ���̾�Ʈ���� �÷��̾� ���� �۽�
	for (const PlayerInfo& otherPlayerInfo : ClientInfo) {
		if (otherPlayerInfo.id != player.id) {
			CObject* pOtherPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(otherPlayerInfo.id));

			SC_PLAYER_PACKET otherPlayerPacket;
			otherPlayerPacket.type			= static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			otherPlayerPacket.playerID		= otherPlayerInfo.id;
			otherPlayerPacket.playerPos		= pOtherPlayer->GetPos();
			otherPlayerPacket.playerState	= ((CPlayer*)pOtherPlayer)->GetState();
			otherPlayerPacket.playerDir		= ((CPlayer*)pOtherPlayer)->GetDir();
			otherPlayerPacket.character		= ((CPlayer*)pOtherPlayer)->GetType();
			size = sizeof(otherPlayerPacket);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&otherPlayerPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				return 0;
			}
		}
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
	{
		std::lock_guard<std::mutex> lock{ g_mutex };	// ���ؽ� ���
		ClientInfo[player.id].isInit = true;			// Ŭ���̾�Ʈ �ʱ�ȭ ���� true�� ����
		std::cout << "[Player" << player.id << "] �������� �ʱ�ȭ �Ϸ�" << std::endl;
	}

	// ��� Ŭ���̾�Ʈ�� �ʱ�ȭ�� �Ϸ��ϸ� �÷��̾�鿡�� ���� ���� ��ȣ ��Ŷ ����
	while (1) {
		std::lock_guard<std::mutex> lock{ g_mutex };
		isAllInit = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isInit) {
				isAllInit = false;
				break;
			}
		}

		if (isAllInit && ClientInfo.size() == MAX_PLAYER)
			break;
	}

	SC_GAME_START_PACKET startPacket;
	startPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
	size = sizeof(SC_GAME_START_PACKET);

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&startPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "��� �÷��̾� �ʱ�ȭ �Ϸ�" << std::endl;
	std::cout << "�÷��̾�鿡�� ���� ���� ��ȣ ��Ŷ ����" << std::endl;

	while (1) {
		Sleep(100 / 60);	// 600fps
		// ���� ����
		memset(buf, 0, BUFSIZE);

		////////////
		// recv() //
		////////////

		// ������ �ޱ�
		retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		retval = recv(client_sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			//�޾ƿ� ��Ŷ�� Ű���� �Է� ��Ŷ���� ĳ����
			CS_KEYBOARD_INPUT_PACKET* pKeyInputPacket = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);

			for (int i = 0; i < pKeyInputPacket->keyCount; ++i) {
				// NONE�� �ƴ� ����	ó��
				if (pKeyInputPacket->inputs[i].key != KEY::LAST && pKeyInputPacket->inputs[i].key_state != KEY_STATE::NONE) {
					// �÷��̾� ��ġ �ڱ�͸� ����
					// �� �÷��̾��� ��ġ ������Ʈ�� �� Ŭ���̾�Ʈ�� �����忡�� �迭�� ���������ؼ� ���� �ٲ�
					Vec2 vCurPos = pCharacter->GetPos();
					Vec2 vDummyPos{};
					float speed = ((CPlayer*)pCharacter)->GetSpeed();

					// ��
					if (pKeyInputPacket->inputs[i].key == KEY::UP && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x, vCurPos.y - speed * DT);

						if (IsInWorld(vDummyPos))
							vCurPos.y -= speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.y -= speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::UP && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					// ��
					if (pKeyInputPacket->inputs[i].key == KEY::DOWN && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x, vCurPos.y + speed * DT);

						if (IsInWorld(vDummyPos))
							vCurPos.y += speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.y += speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::DOWN && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					// ��
					if (pKeyInputPacket->inputs[i].key == KEY::LEFT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x - speed * DT, vCurPos.y);

						if (IsInWorld(vDummyPos))
							vCurPos.x -= speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.x -= speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

						if (((CPlayer*)pCharacter)->GetDir() != DIR_LEFT)
							((CPlayer*)pCharacter)->SetDir(DIR_LEFT);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::LEFT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					// ��
					if (pKeyInputPacket->inputs[i].key == KEY::RIGHT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x + speed * DT, vCurPos.y);

						if (IsInWorld(vDummyPos))
							vCurPos.x += speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.x += speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

						if (((CPlayer*)pCharacter)->GetDir() != DIR_RIGHT)
							((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::RIGHT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					if (pKeyInputPacket->inputs[i].key == KEY::SPACE && pKeyInputPacket->inputs[i].key_state == KEY_STATE::TAP) {
						// �Ѿ� �߻�
						((CPlayer*)pCharacter)->CreateBullet(player.id);
					}

					// ó���� ���� ������Ʈ
					((CPlayer*)pCharacter)->SetPos(vCurPos);
				}
			}

			// �÷��̾� ���� �۽�
			SC_PLAYER_PACKET playerPacket;
			playerPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			playerPacket.playerID = player.id;
			playerPacket.playerPos = pCharacter->GetPos();
			playerPacket.playerState = ((CPlayer*)pCharacter)->GetState();
			playerPacket.playerDir = ((CPlayer*)pCharacter)->GetDir();
			size = sizeof(playerPacket);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&playerPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// ������ ������ �ٸ� Ŭ���̾�Ʈ ���� �۽�
			for (const PlayerInfo& otherPlayerInfo : ClientInfo) {
				if (otherPlayerInfo.id != player.id) {
					CObject* pOtherPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(otherPlayerInfo.id));

					SC_PLAYER_PACKET otherPlayerPacket;
					otherPlayerPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
					otherPlayerPacket.playerID = otherPlayerInfo.id;
					otherPlayerPacket.playerPos = pOtherPlayer->GetPos();
					otherPlayerPacket.playerState = ((CPlayer*)pOtherPlayer)->GetState();
					otherPlayerPacket.playerDir = ((CPlayer*)pOtherPlayer)->GetDir();
					otherPlayerPacket.character = ((CPlayer*)pOtherPlayer)->GetType();
					size = sizeof(otherPlayerPacket);

					retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
					retval = send(client_sock, reinterpret_cast<char*>(&otherPlayerPacket), size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						return 0;
					}
				}
			}
		}

		////////////
		// send() //
		////////////

		// ���� ���� �۽�
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecMonster = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::MONSTER);
			
			SC_MONSTER_PACKET monsterPacket;
			monsterPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
			size = sizeof(SC_MONSTER_PACKET);

			for (const CObject* pMonster : vecMonster) {
				// ���� ������ ��´�
				monsterPacket.monsterPos	= ((CMonster*)pMonster)->GetPos();
				monsterPacket.monsterState	= ((CMonster*)pMonster)->GetState();
				monsterPacket.monsterDir	= ((CMonster*)pMonster)->GetDir();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&monsterPacket), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}
		
		// ������ ���� �۽�
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecItem = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::ITEM);

			SC_ITEM_PACKET itemPacket;
			itemPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
			size = sizeof(SC_ITEM_PACKET);

			// ������ �� ����
			int itemCount = vecItem.size();
			retval = send(client_sock, reinterpret_cast<char*>(&itemCount), sizeof(itemCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// ������ ���� ������
			for (CObject* pItem : vecItem) {
				itemPacket.itemPos		= ((CItem*)pItem)->GetPos();
				itemPacket.itemIsDead	= pItem->IsDead();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&itemPacket), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				// ������ ����
				if (pItem->IsDead()) {
					// itemCount ����
					--itemCount;
				}
			}
		}
		
		// �÷��̾� ����ü ���� �۽�
		//{
		//	std::lock_guard<std::mutex> lock{ g_mutex };
		//	const std::vector<CObject*>& vecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_PLAYER);

		//	SC_BULLET_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
		//	size = sizeof(SC_BULLET_PACKET);

		//	// Ŭ���̾�Ʈ���� �÷��̾� ����ü �� ����
		//	int bulletCount = vecBullet.size();
		//	retval = send(client_sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	// Ŭ���̾�Ʈ�� �÷��̾� ����ü ���� ������
		//	for (CObject* pBullet : vecBullet) {
		//		p.bulletPos = ((CBullet*)pBullet)->GetPos();
		//		p.bulletIsDead = ((CBullet*)pBullet)->IsDead();
		//		p.bulletDir = ((CBullet*)pBullet)->GetDir();

		//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//		if (retval == SOCKET_ERROR) {
		//			err_display("send()");
		//			break;
		//		}

		//		// ����ü ����
		//		if (pBullet->IsDead()) {
		//			// BulletCount ����
		//			--bulletCount;
		//		}
		//	}
		//}
		
	}

	std::cout << "\n[TCP ����] Ŭ���̾�Ʈ ���� ����: IP �ּ�=" << addr << ", ��Ʈ ��ȣ=" << ntohs(clientaddr.sin_port) << ", �г���=" << nick_name << std::endl;

	closesocket(client_sock);

	--clientId;

	// Ŭ���̾�Ʈ ���Ϳ��� �ش� Ŭ���̾�Ʈ ���� ����
	{
		std::lock_guard<std::mutex> lock{ g_mutex };
		ClientInfo.erase(std::remove_if(ClientInfo.begin(), ClientInfo.end(), [&](const PlayerInfo& info) {
			return info.id == player.id;
		}), ClientInfo.end());

		// Ŭ���̾�Ʈ ���Ϳ��� �ش� Ŭ���̾�Ʈ ���� ���� �� ���� Ŭ���̾�Ʈ ���� ����
		for (int i = 0; i < ClientInfo.size(); ++i)
			ClientInfo[i].id = i;

		// ���� Ŭ���̾�Ʈ ����	���
		std::cout<< "���� Ŭ���̾�Ʈ ����" << std::endl;
		for (const PlayerInfo& info : ClientInfo)
			std::cout << "id: " << info.id << ", nickname: " << info.nickname << ", portnumber: " << info.portnumber << std::endl;

		// Ŭ���̾�Ʈ ���Ϳ� ���� Ŭ���̾�Ʈ�� ������ Ŭ���̾�Ʈ ���� �ʱ�ȭ
		if (ClientInfo.empty())
			ClientInfo.clear();
	}
	
	delete pCharacter;
	
	return 0;
}

void init()
{
	// Ÿ�̸� �ʱ�ȭ
	CTimer::GetInst()->init();

	// ���� �����
	CreateMonsters();

	// ���� �����
	CreateBoss();

	// ������ �����
	CreateItems();

	// �浹 �׷� ����
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::ITEM, GROUP_TYPE::PLAYER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::MONSTER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::BOSS);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_BOSS, GROUP_TYPE::PLAYER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_MONSTER, GROUP_TYPE::PLAYER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MISSILE_BOSS, GROUP_TYPE::PLAYER);
}

DWORD WINAPI Progress(LPVOID arg) 
{
	// �ʱ�ȭ
	init();

	while (1) {
		Sleep(100 / 60);	// 600fps
		// �Ŵ�¡ ���⿡�� ó��
		// Ÿ�̸�
		CTimer::GetInst()->update();
		
		//����
		CObjectMgr::GetInst()->DeleteDeadObject();

		// ������Ʈ
		CObjectMgr::GetInst()->update();

		// �浹
		CCollisionMgr::GetInst()->update();

		// �̺�Ʈ
		CEventMgr::GetInst()->update();
	}

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

	// 4�� ������
	HANDLE hThread4 = CreateThread(NULL, 0, Progress, NULL, 0, NULL);

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// Ŭ���̾�Ʈ�� ������ �����忡�� ó��
		std::lock_guard<std::mutex> lock{ g_mutex };
		HANDLE hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		if (hThread == NULL)
			closesocket(client_sock); // �Լ� ���н� ���� �ݱ�
		else
			CloseHandle(hThread);
	}

	CloseHandle(hThread4);

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
