#include "pch.h"
#include "CCore.h"
#include "CObject.h"

#include "CTimer.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CScene.h"
#include "CSound.h"
#include "CCollider.h"
#include "CPlayer.h"

bool IsGameClear = false;
bool IsGameOver = false;

CCore::CCore() :
	m_hWnd(nullptr), m_ptResolution{}, m_hDC(nullptr),
	m_hBit(nullptr), m_memDC(nullptr), m_arrBrush{}, m_arrPen{}, 
	m_sock{ INVALID_SOCKET }, m_bIsStart{ false }
{
	// Ű ���� �ʱ�ȭ
	for (int i = 0; i < (int)KEY::LAST; ++i) {
		m_inputkey[i].inputs->key = KEY(i);
		m_inputkey[i].inputs->key_state = KEY_STATE::NONE;
	}
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	// ��� �귯�� ����
	for (HPEN& _hPen : m_arrPen)
		DeleteObject(_hPen);

	for (HBRUSH& _hBrush : m_arrBrush)
		DeleteObject(_hBrush);
}


int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// �ػ󵵿� �°� ������ ũ�� ����
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	// �����츦 ũ�⸦ ������ �� ����, �ִ�ȭ ��ư�� ���ֵ��� �����.
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_SIZEBOX);
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

	// �����쿡 �׸��� �׷��� DC�� �����
	m_hDC = GetDC(m_hWnd);

	// ���� ���۸� �뵵�� ��Ʈ�ʰ� DC�� �����.
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	// ���� ����� �� �� �귯�� ����
	CreateBrushPen();

	// Init Managers
	CPathMgr::GetInst()->init();
	CTimer::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CSoundMgr::GetInst()->init();

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return S_FALSE;

	return S_OK;
}

void CCore::CreateBrushPen()
{
	// hollow brush (���� �귯��)
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// red pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	// green pen
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	// blue pen
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

void CCore::TestSendKeyInput()
{
	char buf[BUFSIZE];
	int retval;
	int size;
	bool bAllKeyNone = true;
	SOCKET sock = m_sock;

	if (!IsGameClear && !IsGameOver) {
		CS_KEYBOARD_INPUT_PACKET cs_p;
		cs_p.type = static_cast<char>(CS_PACKET_TYPE::CS_KEYBOARD_INPUT);
		cs_p.keyCount = 0;
		size = sizeof(cs_p);

		for (int i = 0; i < (int)KEY::LAST; ++i) {
			if (m_inputkey[i].inputs->key_state != KEY_STATE::NONE) {
				if (bAllKeyNone)
					bAllKeyNone = false;

				if (cs_p.keyCount < MAX_KEYS) {
					cs_p.inputs[cs_p.keyCount].key = m_inputkey[i].inputs->key;
					cs_p.inputs[cs_p.keyCount].key_state = m_inputkey[i].inputs->key_state;
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

		////////////
		// recv() //
		////////////

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

		// ���� ���� �ޱ�
		SC_MONSTER_PACKET* pMonsterPacket = reinterpret_cast<SC_MONSTER_PACKET*>(buf);

		// ���� �� �ޱ�
		int monsterCount = 0;
		retval = recv(sock, reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
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
			if (pBossPacket->bossState != BOSS_STATE::NOT_APPEAR) {
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
			IsGameClear = true;
			IsGameOver = false;
		}
		else if (buf[0] == static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER)) {
			// ���� ����
			ChangeScene(SCENE_TYPE::GAMEOVER);
			IsGameOver = true;
			IsGameClear = false;
		}
	}
}

void CCore::progress()
{
	// ������ ����Ǿ� ������ ���
	if (m_sock != INVALID_SOCKET && m_bIsStart) {
		TestSendKeyInput();
	}

	// Managers Update
	CTimer::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();

	CSceneMgr::GetInst()->update();

	///////////////
	// Rendering //
	///////////////
	
	// ȭ�� Clear
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	// �׸���
	CSceneMgr::GetInst()->render(m_memDC);

	// �ĸ������ ������ ������� �ű��.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY/* �״�� ���� */);

	// Ÿ��Ʋ �ٿ� ������ render
	CTimer::GetInst()->render();

	// �̺�Ʈ�� �� �������� ������Ʈ (����ó��)
	CEventMgr::GetInst()->update();
}