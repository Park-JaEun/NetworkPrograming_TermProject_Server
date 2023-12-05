#include "pch.h"
#include "functions.h"

#include "CObject.h"
#include "CFloor.h"
#include "CMonster.h"
#include "CItem.h"

#include "CCollider.h"

#include "CEventMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CScene_Select.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent event{};
	event.eEvent = EVENT_TYPE::CREATE_OBJECT;
	event.lParam = (DWORD_PTR)_pObj;
	event.wParam = (DWORD_PTR)_eGroup;

	ADDEVENT(event);
}

void CreateMonster(const Vec2& _vPos, int id)
{
	CObject* pMonsterObj = new CMonster;

	pMonsterObj->SetName(L"Monster" + std::to_wstring(id));
	pMonsterObj->SetPos(_vPos);
	((CMonster*)pMonsterObj)->SetFirstPos(pMonsterObj->GetPos()); // SetFirstPos�� ����� �� �ֵ��� CMonster�� ĳ����

	// �浹ü, �ִϸ����� ����
	pMonsterObj->CreateCollider();
	pMonsterObj->CreateAnimator();

	// �浹ü ũ�� ����
	pMonsterObj->GetCollider()->SetScale(Vec2(30.f, 48.f));

	CreateObject(pMonsterObj, GROUP_TYPE::MONSTER);
}

void CreateFloor(const Vec2& _vPos)
{
	CObject* pFloorObj = new CFloor;

	pFloorObj->SetName(L"Floor");
	pFloorObj->SetPos(_vPos);

	// �ִϸ����� ����
	pFloorObj->CreateAnimator();

	CreateObject(pFloorObj, GROUP_TYPE::FLOOR);
}

void CreateBunny(const Vec2& _vPos, int id)
{
	CItem* pBunnyItemObj = new CItem;

	pBunnyItemObj->SetName(L"Bunny" + std::to_wstring(id));
	pBunnyItemObj->SetPos(_vPos);
	pBunnyItemObj->SetFirstPos(_vPos);
	pBunnyItemObj->SetScale(Vec2(25.f, 25.f));
	pBunnyItemObj->SetType(ITEM_TYPE::BUNNY);

	pBunnyItemObj->CreateCollider();
	pBunnyItemObj->GetCollider()->SetScale(Vec2(25.f, 25.f));
	pBunnyItemObj->GetCollider()->SetOffsetPos(Vec2(12.5f, 12.5f));

	CreateObject(pBunnyItemObj, GROUP_TYPE::ITEM_RABBIT);
}

void CreateCookie(const Vec2& _vPos, int id)
{
	CItem* pCookieItemObj = new CItem;

	pCookieItemObj->SetName(L"Cookie" + std::to_wstring(id));
	pCookieItemObj->SetPos(_vPos);
	pCookieItemObj->SetFirstPos(_vPos);
	pCookieItemObj->SetScale(Vec2(25.f, 25.f));
	pCookieItemObj->SetType(ITEM_TYPE::COOKIE);

	pCookieItemObj->CreateCollider();
	pCookieItemObj->GetCollider()->SetScale(Vec2(25.f, 25.f));
	pCookieItemObj->GetCollider()->SetOffsetPos(Vec2(12.5f, 12.5f));

	CreateObject(pCookieItemObj, GROUP_TYPE::ITEM_COOKIE);
}

void DeleteObject(CObject* _pObj)
{
	tEvent event{};
	event.eEvent = EVENT_TYPE::DELETE_OBJECT;
	event.lParam = (DWORD_PTR)_pObj;
	event.wParam = (DWORD_PTR)nullptr;

	ADDEVENT(event);
}

void ChangeScene(SCENE_TYPE _eNextScene)
{
	tEvent event{};
	event.eEvent = EVENT_TYPE::SCENE_CHANGE;
	event.lParam = (DWORD_PTR)_eNextScene;
	event.wParam = (DWORD_PTR)nullptr;

	ADDEVENT(event);
}

bool IsInWorld(const Vec2& _vPos)
{
	if (_vPos.y < 184.f && _vPos.y > -184.f && _vPos.x < 5359.f && _vPos.x > -304.f)
		return true;
	return false;
}

bool IsInBossRoom(const Vec2& _vPos)
{
	if (_vPos.y < 184.f && _vPos.y > -184.f && _vPos.x < 5365.f && _vPos.x > 4755.f)
		return true;
	return false;
}

bool IsInCamera(const Vec2& _vPos)
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCameraLookAt = CCamera::GetInst()->GetLookAt();

	if (_vPos.y < vCameraLookAt.y + (vResolution.y / 2.f) && 
		_vPos.y > vCameraLookAt.y - (vResolution.y / 2.f) &&
		_vPos.x < vCameraLookAt.x + (vResolution.x / 2.f) &&
		_vPos.x > vCameraLookAt.x - (vResolution.x / 2.f))
		return true;
	return false;
}

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

float LerpX(float start, float end, float alpha) {
	return start + (end - start) * alpha;
}

float LerpY(float start, float end, float alpha) {
	return start + (end - start) * alpha;
}

// �÷��̾� ID�� �����ϴ� �Լ� 
void setId(HWND hEditNickname, HWND hEditIP)
{
	int iLenNickname = GetWindowTextLength(hEditNickname);
	int iLenIP = GetWindowTextLength(hEditIP);
	int iPlayerID{};	// �÷��̾� ID
	int retval{};		// ���ϰ�
	int size{};			// ������ ������ ũ��
	char buf[BUFSIZE];	// ���� ����

	TCHAR* pStrNickname = new TCHAR[iLenNickname + 1];
	TCHAR* pStrIP = new TCHAR[iLenIP + 1];

	// ������ IP�� ���� �г����� �����´�.
	GetWindowText(hEditNickname, pStrNickname, iLenNickname + 1);
	GetWindowText(hEditIP, pStrIP, iLenIP + 1);

	// TCHAR -> char
	char* pStrIPChar = new char[iLenIP + 1];
	char* pStrNicknameChar = new char[iLenNickname + 1];

	for (int i = 0; i < iLenIP + 1; ++i) {
		pStrIPChar[i] = pStrIP[i];
	}

	for (int i = 0; i < iLenNickname + 1; ++i) {
		pStrNicknameChar[i] = pStrNickname[i];
	}

	// ������ ����
	SOCKET sock = CCore::GetInst()->GetSocket();

	// ���� ���� �� �Ҵ�
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");
	CCore::GetInst()->SetSocket(sock);

	// ���� �ּ� ����
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, pStrIPChar, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		err_quit("connect()");

	// �г��� �۽�
	retval = send(sock, reinterpret_cast<char*>(&iLenNickname), sizeof(int), 0);
	retval = send(sock, (char*)pStrNicknameChar, iLenNickname, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		closesocket(sock);
		WSACleanup();
		// ������ ��������
		PostQuitMessage(0);
	}
	std::cout << "�г��� �۽�" << std::endl;

	// �÷��̾� ID ����
	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		PostQuitMessage(0);
	}

	SC_MAKE_ID_PACKET* IDPacket = reinterpret_cast<SC_MAKE_ID_PACKET*>(buf);
	std::cout << "Player" << IDPacket->id << " ���� ����" << std::endl;

	// �÷��̾� ID ����
	CCore::GetInst()->SetID(IDPacket->id);

	// �÷��̾� �г��� ����
	CCore::GetInst()->SetNickName(pStrNickname);

	delete[] pStrNickname;
	delete[] pStrIP;
	delete[] pStrIPChar;
	delete[] pStrNicknameChar;
}

void sendSelectCharacter(SOCKET sock)
{
	SELECT_CHARACTER_PACKET selectPacket;
	CHARACTER_TYPE eSelectedCharacter;
	
	char buf[BUFSIZE]{};
	int retval{};
	int size = sizeof(SELECT_CHARACTER_PACKET);

	CScene* curScene = CSceneMgr::GetInst()->GetCurScene();
	eSelectedCharacter = ((CScene_Select*)curScene)->GetSelectedCharacter();

	selectPacket.type = static_cast<char>(CS_PACKET_TYPE::SELECT_CHARACTER);
	selectPacket.character = eSelectedCharacter;
	selectPacket.id = CCore::GetInst()->GetID();

	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&selectPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		closesocket(sock);
		WSACleanup();
		PostQuitMessage(0);
	}
	std::cout << "ĳ���� ���� ��Ŷ �۽�" << std::endl;
}

bool recvInitSignal(SOCKET sock)
{
	char buf[BUFSIZE]{};
	int retval{};
	int size;

	retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = recv(sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		PostQuitMessage(0);
	}
	std::cout << "�������� �ʱ�ȭ ��Ŷ ����" << std::endl;
	SC_INIT_PACKET* initPacket = reinterpret_cast<SC_INIT_PACKET*>(buf);
	
	if (initPacket->type == static_cast<char>(SC_PACKET_TYPE::SC_INIT))
		return true;
	
	return false;
}

void recvPlayerInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// �÷��̾�� ���� �ޱ�
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	SC_PLAYER_PACKET* pPlayerPacket = reinterpret_cast<SC_PLAYER_PACKET*>(buf);
	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	CObject* pPlayer = pCurScene->FindObject(L"Player" + std::to_wstring(pPlayerPacket->playerID));
	((CPlayer*)pPlayer)->SetPos(pPlayerPacket->playerPos);
	((CPlayer*)pPlayer)->SetState(pPlayerPacket->playerState);
	((CPlayer*)pPlayer)->SetDir(pPlayerPacket->playerDir);
	((CPlayer*)pPlayer)->SetLife(pPlayerPacket->playerLife);
	((CPlayer*)pPlayer)->SetHP(pPlayerPacket->playerHP);
	((CPlayer*)pPlayer)->SetBunnyCount(pPlayerPacket->playerBunnyCount);
	((CPlayer*)pPlayer)->SetCookieCount(pPlayerPacket->playerCookieCount);
	((CPlayer*)pPlayer)->SetKillCount(pPlayerPacket->playerKillCount);

	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	pPlayer = pCurScene->FindObject(L"Player" + std::to_wstring(pPlayerPacket->playerID));
	((CPlayer*)pPlayer)->SetPos(pPlayerPacket->playerPos);
	((CPlayer*)pPlayer)->SetState(pPlayerPacket->playerState);
	((CPlayer*)pPlayer)->SetDir(pPlayerPacket->playerDir);
	((CPlayer*)pPlayer)->SetLife(pPlayerPacket->playerLife);
	((CPlayer*)pPlayer)->SetHP(pPlayerPacket->playerHP);
	((CPlayer*)pPlayer)->SetBunnyCount(pPlayerPacket->playerBunnyCount);
	((CPlayer*)pPlayer)->SetCookieCount(pPlayerPacket->playerCookieCount);
	((CPlayer*)pPlayer)->SetKillCount(pPlayerPacket->playerKillCount);

	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	pPlayer = (CPlayer*)pCurScene->FindObject(L"Player" + std::to_wstring(pPlayerPacket->playerID));
	((CPlayer*)pPlayer)->SetPos(pPlayerPacket->playerPos);
	((CPlayer*)pPlayer)->SetState(pPlayerPacket->playerState);
	((CPlayer*)pPlayer)->SetDir(pPlayerPacket->playerDir);
	((CPlayer*)pPlayer)->SetLife(pPlayerPacket->playerLife);
	((CPlayer*)pPlayer)->SetHP(pPlayerPacket->playerHP);
	((CPlayer*)pPlayer)->SetBunnyCount(pPlayerPacket->playerBunnyCount);
	((CPlayer*)pPlayer)->SetCookieCount(pPlayerPacket->playerCookieCount);
	((CPlayer*)pPlayer)->SetKillCount(pPlayerPacket->playerKillCount);
}


void recvMonsterInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// ���� ���� �ޱ�
	SC_MONSTER_PACKET* pMonsterPacket = reinterpret_cast<SC_MONSTER_PACKET*>(buf);

	// ���� �� �ޱ�
	int monsterCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	if (monsterCount != 0) {
		// ���� �ޱ�
		for (int i = 0; i < monsterCount; ++i) {
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			// vecMonster �ȿ� �Ȱ��� �̸��� ������Ʈ �ҷ�����
			CObject* pMonster = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Monster" + std::to_wstring(pMonsterPacket->monsterID));

			if (pMonsterPacket->monsterIsDead) {
				// ������ ���ʹ� Ŭ���̾�Ʈ ������ ����
				DeleteObject(pMonster);
			}
			else {
				// �������� ���� ���͵��� ������Ʈ
				pMonster->SetPos(pMonsterPacket->monsterPos);
				((CMonster*)pMonster)->SetState(pMonsterPacket->monsterState);
				((CMonster*)pMonster)->SetDir(pMonsterPacket->monsterDir);
			}
		}
	}
}


void recvBossInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// ���� ���� �ޱ�
	SC_BOSS_PACKET* pBossPacket = reinterpret_cast<SC_BOSS_PACKET*>(buf);

	// ���� �� �ޱ�
	int bossCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&bossCount), sizeof(bossCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	// ���� ���� �ޱ�
	if (bossCount != 0) {
		retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
		retval = recv(sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		// vecBoss �ȿ� �Ȱ��� �̸��� ������Ʈ �ҷ�����
		CObject* pBoss = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Boss");
		if (pBossPacket->bossIsDead) {
			// ������ ������ Ŭ���̾�Ʈ ������ ����
			DeleteObject(pBoss);
		}
		else {
			// �������� ���� �������� ������Ʈ
			pBoss->SetPos(pBossPacket->bossPos);
			((CBoss*)pBoss)->SetState(pBossPacket->bossState);
		}
	}
}


void recvRabbitItemInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// �䳢 ������ ���� �ޱ�
	SC_RABBIT_ITEM_PACKET* pRabbitItemPacket = reinterpret_cast<SC_RABBIT_ITEM_PACKET*>(buf);

	// �䳢 ������ �� �ޱ�
	int rabbitItemCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&rabbitItemCount), sizeof(rabbitItemCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (rabbitItemCount != 0) {
		// ���� �ޱ�
		for (int i = 0; i < rabbitItemCount; ++i) {
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			// vecRabbitItem �ȿ� �Ȱ��� �̸��� ������Ʈ �ҷ�����
			CObject* pRabbitItem = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Bunny" + std::to_wstring(pRabbitItemPacket->itemID));

			if (pRabbitItemPacket->itemIsDead) {
				// ������ �������� Ŭ���̾�Ʈ ������ ����
				DeleteObject(pRabbitItem);
			}
			else {
				// �������� ���� �����۵��� ������Ʈ
				pRabbitItem->SetPos(pRabbitItemPacket->itemPos);
			}
		}
	}
}

void recvCookieInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// ��Ű ������ ���� �ޱ�
	SC_COOKIE_ITEM_PACKET* pCookieItemPacket = reinterpret_cast<SC_COOKIE_ITEM_PACKET*>(buf);

	// ��Ű ������ �� �ޱ�
	int cookieItemCount = 0;

	retval = recv(sock, reinterpret_cast<char*>(&cookieItemCount), sizeof(cookieItemCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (cookieItemCount != 0) {
		// ���� �ޱ�
		for (int i = 0; i < cookieItemCount; ++i) {
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			// vecCookieItem �ȿ� �Ȱ��� �̸��� ������Ʈ �ҷ�����
			CObject* pCookieItem = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Cookie" + std::to_wstring(pCookieItemPacket->itemID));

			if (pCookieItemPacket->itemIsDead) {
				// ������ �������� Ŭ���̾�Ʈ ������ ����
				DeleteObject(pCookieItem);
			}
			else {
				// �������� ���� �����۵��� ������Ʈ
				pCookieItem->SetPos(pCookieItemPacket->itemPos);
			}
		}
	}
}


void recvPlayerBulletInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// �÷��̾� ����ü ���� �ޱ�
	SC_BULLET_PACKET* pbulletPacket = reinterpret_cast<SC_BULLET_PACKET*>(buf);

	// �÷��̾� ����ü �� �ޱ�
	int bulletCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (bulletCount != 0) {
		// ���� �ޱ�
		for (int i = 0; i < bulletCount; ++i) {
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			// vecPlayerBullet �ȿ� �Ȱ��� �̸��� ������Ʈ�� �ִ��� Ȯ��
			CObject* pBullet = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Player" + std::to_wstring(pbulletPacket->playerID) + L"Bullet" + std::to_wstring(pbulletPacket->bulletID));

			if (pBullet == nullptr) {
				// ������ ���� ����
				pBullet = new CBullet(pbulletPacket->bulletPos);
				pBullet->SetName(L"Player" + std::to_wstring(pbulletPacket->playerID) + L"Bullet" + std::to_wstring(pbulletPacket->bulletID));
				pBullet->SetPos(pbulletPacket->bulletPos);
				((CBullet*)pBullet)->SetFirstPos(pbulletPacket->bulletPos);
				((CBullet*)pBullet)->SetDir(pbulletPacket->bulletDir);
				((CBullet*)pBullet)->SetSpeed(700.f);

				pBullet->CreateCollider();
				((CBullet*)pBullet)->CreateAnimator(pbulletPacket->playerID);

				pBullet->GetCollider()->SetScale(Vec2(25.f, 25.f));

				CreateObject(pBullet, GROUP_TYPE::BULLET_PLAYER);

				// �÷��̾� ���� ����Ʈ ���
				CObject* pPlayer = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Player" + std::to_wstring(pbulletPacket->playerID));
				((CPlayer*)pPlayer)->PlayShootingEffect();
			}
			// ������ ������Ʈ
			else {
				if (pbulletPacket->bulletIsDead) {
					// ������ �������� Ŭ���̾�Ʈ ������ ����
					DeleteObject(pBullet);
				}
				else {
					// �������� ���� �����۵��� ������Ʈ
					pBullet->SetPos(pbulletPacket->bulletPos);
				}
			}
		}
	}
}

void recvMonsterBulletInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// ���� ����ü ���� �ޱ�
	SC_BULLET_PACKET* pMonsterBulletPacket = reinterpret_cast<SC_BULLET_PACKET*>(buf);

	// ���� ����ü �� �ޱ�
	int monsterBulletCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&monsterBulletCount), sizeof(monsterBulletCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (monsterBulletCount != 0) {
		// ���� �ޱ�
		for (int i = 0; i < monsterBulletCount; ++i) {
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			// vecMonsterBullet �ȿ� �Ȱ��� �̸��� ������Ʈ�� �ִ��� Ȯ��
			CObject* pBullet = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"MonsterBullet" + std::to_wstring(pMonsterBulletPacket->bulletID));

			if (pBullet == nullptr) {
				// ������ ���� ����
				pBullet = new CBullet(pMonsterBulletPacket->bulletPos);
				pBullet->SetName(L"MonsterBullet" + std::to_wstring(pMonsterBulletPacket->bulletID));
				pBullet->SetPos(pMonsterBulletPacket->bulletPos);
				((CBullet*)pBullet)->SetFirstPos(pMonsterBulletPacket->bulletPos);
				((CBullet*)pBullet)->SetDir(pMonsterBulletPacket->bulletDir);
				((CBullet*)pBullet)->SetSpeed(700.f);

				pBullet->CreateCollider();
				((CBullet*)pBullet)->CreateAnimator(GROUP_TYPE::BULLET_MONSTER);

				pBullet->GetCollider()->SetScale(Vec2(18.f, 18.f));

				CreateObject(pBullet, GROUP_TYPE::BULLET_MONSTER);
			}
			// ������ ������Ʈ
			else {
				if (pMonsterBulletPacket->bulletIsDead) {
					// ������ �������� Ŭ���̾�Ʈ ������ ����
					DeleteObject(pBullet);
				}
				else {
					// �������� ���� �����۵��� ������Ʈ
					pBullet->SetPos(pMonsterBulletPacket->bulletPos);
				}
			}
		}
	}
}


void recvBossBulletInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// ���� ����ü ���� �ޱ�
	SC_BULLET_PACKET* pBossBulletPacket = reinterpret_cast<SC_BULLET_PACKET*>(buf);

	// ���� ����ü �� �ޱ�
	int bossBulletCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&bossBulletCount), sizeof(bossBulletCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (bossBulletCount != 0) {
		// ���� �ޱ�
		for (int i = 0; i < bossBulletCount; ++i) {
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			// vecBossBullet �ȿ� �Ȱ��� �̸��� ������Ʈ�� �ִ��� Ȯ��
			CObject* pBullet = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"BossBullet" + std::to_wstring(pBossBulletPacket->bulletID));

			if (pBullet == nullptr) {
				// ������ ���� ����
				pBullet = new CBullet(pBossBulletPacket->bulletPos);
				pBullet->SetName(L"BossBullet" + std::to_wstring(pBossBulletPacket->bulletID));
				pBullet->SetPos(pBossBulletPacket->bulletPos);
				((CBullet*)pBullet)->SetFirstPos(pBossBulletPacket->bulletPos);
				((CBullet*)pBullet)->SetDir(pBossBulletPacket->bulletDir);
				((CBullet*)pBullet)->SetDegree(pBossBulletPacket->bulletDegree);
				((CBullet*)pBullet)->SetSpeed(100.f);

				pBullet->CreateCollider();
				((CBullet*)pBullet)->CreateAnimator(GROUP_TYPE::BULLET_BOSS);

				pBullet->GetCollider()->SetScale(Vec2(8.f, 8.f));

				CreateObject(pBullet, GROUP_TYPE::BULLET_BOSS);
			}
			// ������ ������Ʈ
			else {
				if (pBossBulletPacket->bulletIsDead) {
					// ������ �������� Ŭ���̾�Ʈ ������ ����
					DeleteObject(pBullet);
				}
				else {
					// �������� ���� �����۵��� ������Ʈ
					pBullet->SetPos(pBossBulletPacket->bulletPos);
				}
			}
		}
	}
}


void recvGameStateSignal(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// ���� ���� ��Ŷ �ޱ�
	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	// ��Ŷ Ÿ�� �˻�
	if (buf[0] == static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR)) {
		// ���� Ŭ����
		ChangeScene(SCENE_TYPE::CLEAR);
		CCore::GetInst()->SetGameClear(true);
		CCore::GetInst()->SetGameOver(false);
	}
	else if (buf[0] == static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER)) {
		// ���� ����
		ChangeScene(SCENE_TYPE::GAMEOVER);
		CCore::GetInst()->SetGameOver(true);
		CCore::GetInst()->SetGameClear(false);
	}
}

void sendInitFinishSignal(SOCKET sock)
{
	int retval;
	CS_INIT_FINISH_PACKET initFinishPacket;
	int size = sizeof(CS_INIT_FINISH_PACKET);

	initFinishPacket.type = static_cast<char>(CS_PACKET_TYPE::CS_INIT_FINISH);
	initFinishPacket.id = CCore::GetInst()->GetID();

	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&initFinishPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		closesocket(sock);
		WSACleanup();
		PostQuitMessage(0);
	}
	std::cout << "�ʱ�ȭ �Ϸ� ��Ŷ �۽�" << std::endl;

	std::cout << "���� ���� ��Ŷ ���� �����" << std::endl;
}

void sendKeyBoardInput(SOCKET sock)
{
	int retval{};
	int size{};
	bool bAllKeyNone = true;

	CS_KEYBOARD_INPUT_PACKET cs_p;
	cs_p.type = static_cast<char>(CS_PACKET_TYPE::CS_KEYBOARD_INPUT);
	cs_p.keyCount = 0;
	size = sizeof(cs_p);

	for (int i = 0; i < (int)KEY::LAST; ++i) {
		if (CCore::GetInst()->GetKey(i).inputs->key_state != KEY_STATE::NONE) {
			if (bAllKeyNone)
				bAllKeyNone = false;

			if (cs_p.keyCount < MAX_KEYS) {
				cs_p.inputs[cs_p.keyCount].key = CCore::GetInst()->GetKey(i).inputs->key;
				cs_p.inputs[cs_p.keyCount].key_state = CCore::GetInst()->GetKey(i).inputs->key_state;
				cs_p.keyCount++;
			}
		}
	}

	if (cs_p.keyCount > 0 || bAllKeyNone) {
		if (bAllKeyNone) {
			cs_p.inputs[0].key = KEY::LAST;
			cs_p.inputs[0].key_state = KEY_STATE::NONE;
			cs_p.keyCount = 1;
			bAllKeyNone = true;
		}

		size = sizeof(cs_p);
		retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(sock, reinterpret_cast<char*>(&cs_p), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return;
		}
	}
}


void sendLobbySignal(SOCKET sock)
{

}

void sendExitSignal(SOCKET sock)
{
}

