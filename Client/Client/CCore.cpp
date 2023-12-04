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
	// 키 정보 초기화
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

	// 펜과 브러쉬 제거
	for (HPEN& _hPen : m_arrPen)
		DeleteObject(_hPen);

	for (HBRUSH& _hBrush : m_arrBrush)
		DeleteObject(_hBrush);
}


int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 크기 조정
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	// 윈도우를 크기를 변경할 수 없고, 최대화 버튼을 없애도록 만든다.
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_SIZEBOX);
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

	// 윈도우에 그림을 그려줄 DC를 만들기
	m_hDC = GetDC(m_hWnd);

	// 더블 버퍼링 용도의 비트맵과 DC를 만든다.
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	// 자주 사용할 펜 및 브러쉬 생성
	CreateBrushPen();

	// Init Managers
	CPathMgr::GetInst()->init();
	CTimer::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CSoundMgr::GetInst()->init();

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return S_FALSE;

	return S_OK;
}

void CCore::CreateBrushPen()
{
	// hollow brush (투명 브러쉬)
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

		// 플레이어들 정보 받기
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

		// 몬스터 정보 받기
		SC_MONSTER_PACKET* pMonsterPacket = reinterpret_cast<SC_MONSTER_PACKET*>(buf);

		// 몬스터 수 받기
		int monsterCount = 0;
		retval = recv(sock, reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		if (monsterCount != 0) {
			// 정보 받기
			for (int i = 0; i < monsterCount; ++i) {
				retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
				retval = recv(sock, buf, size, MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					closesocket(sock);
					WSACleanup();
					return;
				}

				// vecMonster 안에 똑같은 이름의 오브젝트 불러오기
				CObject* pMonster = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Monster" + std::to_wstring(pMonsterPacket->monsterID));

				if (pMonsterPacket->monsterIsDead) {
					// 삭제된 몬스터는 클라이언트 내에서 삭제
					DeleteObject(pMonster);
				}
				else {
					// 삭제되지 않은 몬스터들은 업데이트
					pMonster->SetPos(pMonsterPacket->monsterPos);
					((CMonster*)pMonster)->SetState(pMonsterPacket->monsterState);
					((CMonster*)pMonster)->SetDir(pMonsterPacket->monsterDir);
				}
			}
		}

		// 보스 정보 받기
		SC_BOSS_PACKET* pBossPacket = reinterpret_cast<SC_BOSS_PACKET*>(buf);

		// 보스 수 받기
		int bossCount = 0;
		retval = recv(sock, reinterpret_cast<char*>(&bossCount), sizeof(bossCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		// 보스 정보 받기
		if (bossCount != 0) {
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			// vecBoss 안에 똑같은 이름의 오브젝트 불러오기
			CObject* pBoss = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Boss");
			if (pBossPacket->bossState != BOSS_STATE::NOT_APPEAR) {
				if (pBossPacket->bossIsDead) {
					// 삭제된 보스는 클라이언트 내에서 삭제
					DeleteObject(pBoss);
				}
				else {
					// 삭제되지 않은 보스들은 업데이트
					pBoss->SetPos(pBossPacket->bossPos);
					((CBoss*)pBoss)->SetState(pBossPacket->bossState);
				}
			}
		}

		// 토끼 아이템 정보 받기
		SC_RABBIT_ITEM_PACKET* pRabbitItemPacket = reinterpret_cast<SC_RABBIT_ITEM_PACKET*>(buf);

		// 토끼 아이템 수 받기
		int rabbitItemCount = 0;
		retval = recv(sock, reinterpret_cast<char*>(&rabbitItemCount), sizeof(rabbitItemCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		if (rabbitItemCount != 0) {
			// 정보 받기
			for (int i = 0; i < rabbitItemCount; ++i) {
				retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
				retval = recv(sock, buf, size, MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					closesocket(sock);
					WSACleanup();
					return;
				}

				// vecRabbitItem 안에 똑같은 이름의 오브젝트 불러오기
				CObject* pRabbitItem = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Bunny" + std::to_wstring(pRabbitItemPacket->itemID));

				if (pRabbitItemPacket->itemIsDead) {
					// 삭제된 아이템은 클라이언트 내에서 삭제
					DeleteObject(pRabbitItem);
				}
				else {
					// 삭제되지 않은 아이템들은 업데이트
					pRabbitItem->SetPos(pRabbitItemPacket->itemPos);
				}
			}
		}

		// 쿠키 아이템 정보 받기
		SC_COOKIE_ITEM_PACKET* pCookieItemPacket = reinterpret_cast<SC_COOKIE_ITEM_PACKET*>(buf);

		// 쿠키 아이템 수 받기
		int cookieItemCount = 0;

		retval = recv(sock, reinterpret_cast<char*>(&cookieItemCount), sizeof(cookieItemCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		if (cookieItemCount != 0) {
			// 정보 받기
			for (int i = 0; i < cookieItemCount; ++i) {
				retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
				retval = recv(sock, buf, size, MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					closesocket(sock);
					WSACleanup();
					return;
				}

				// vecCookieItem 안에 똑같은 이름의 오브젝트 불러오기
				CObject* pCookieItem = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Cookie" + std::to_wstring(pCookieItemPacket->itemID));

				if (pCookieItemPacket->itemIsDead) {
					// 삭제된 아이템은 클라이언트 내에서 삭제
					DeleteObject(pCookieItem);
				}
				else {
					// 삭제되지 않은 아이템들은 업데이트
					pCookieItem->SetPos(pCookieItemPacket->itemPos);
				}
			}
		}


		// 플레이어 투사체 정보 받기
		SC_BULLET_PACKET* pbulletPacket = reinterpret_cast<SC_BULLET_PACKET*>(buf);

		// 플레이어 투사체 수 받기
		int bulletCount = 0;
		retval = recv(sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		if (bulletCount != 0) {
			// 정보 받기
			for (int i = 0; i < bulletCount; ++i) {
				retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
				retval = recv(sock, buf, size, MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					closesocket(sock);
					WSACleanup();
					return;
				}

				// vecPlayerBullet 안에 똑같은 이름의 오브젝트가 있는지 확인
				CObject* pBullet = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Player" + std::to_wstring(pbulletPacket->playerID) + L"Bullet" + std::to_wstring(pbulletPacket->bulletID));

				if (pBullet == nullptr) {
					// 없으면 새로 생성
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

					// 플레이어 슈팅 이펙트 재생
					CObject* pPlayer = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Player" + std::to_wstring(pbulletPacket->playerID));
					((CPlayer*)pPlayer)->PlayShootingEffect();
				}
				// 있으면 업데이트
				else {
					if (pbulletPacket->bulletIsDead) {
						// 삭제된 아이템은 클라이언트 내에서 삭제
						DeleteObject(pBullet);
					}
					else {
						// 삭제되지 않은 아이템들은 업데이트
						pBullet->SetPos(pbulletPacket->bulletPos);
					}
				}
			}
		}

		// 몬스터 투사체 정보 받기
		SC_BULLET_PACKET* pMonsterBulletPacket = reinterpret_cast<SC_BULLET_PACKET*>(buf);

		// 몬스터 투사체 수 받기
		int monsterBulletCount = 0;
		retval = recv(sock, reinterpret_cast<char*>(&monsterBulletCount), sizeof(monsterBulletCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		if (monsterBulletCount != 0) {
			// 정보 받기
			for (int i = 0; i < monsterBulletCount; ++i) {
				retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
				retval = recv(sock, buf, size, MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					closesocket(sock);
					WSACleanup();
					return;
				}

				// vecMonsterBullet 안에 똑같은 이름의 오브젝트가 있는지 확인
				CObject* pBullet = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"MonsterBullet" + std::to_wstring(pMonsterBulletPacket->bulletID));

				if (pBullet == nullptr) {
					// 없으면 새로 생성
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
				// 있으면 업데이트
				else {
					if (pMonsterBulletPacket->bulletIsDead) {
						// 삭제된 아이템은 클라이언트 내에서 삭제
						DeleteObject(pBullet);
					}
					else {
						// 삭제되지 않은 아이템들은 업데이트
						pBullet->SetPos(pMonsterBulletPacket->bulletPos);
					}
				}
			}
		}

		// 보스 투사체 정보 받기
		SC_BULLET_PACKET* pBossBulletPacket = reinterpret_cast<SC_BULLET_PACKET*>(buf);

		// 보스 투사체 수 받기
		int bossBulletCount = 0;
		retval = recv(sock, reinterpret_cast<char*>(&bossBulletCount), sizeof(bossBulletCount), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		if (bossBulletCount != 0) {
			// 정보 받기
			for (int i = 0; i < bossBulletCount; ++i) {
				retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
				retval = recv(sock, buf, size, MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					closesocket(sock);
					WSACleanup();
					return;
				}

				// vecBossBullet 안에 똑같은 이름의 오브젝트가 있는지 확인
				CObject* pBullet = CSceneMgr::GetInst()->GetCurScene()->FindObject(L"BossBullet" + std::to_wstring(pBossBulletPacket->bulletID));

				if (pBullet == nullptr) {
					// 없으면 새로 생성
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
				// 있으면 업데이트
				else {
					if (pBossBulletPacket->bulletIsDead) {
						// 삭제된 아이템은 클라이언트 내에서 삭제
						DeleteObject(pBullet);
					}
					else {
						// 삭제되지 않은 아이템들은 업데이트
						pBullet->SetPos(pBossBulletPacket->bulletPos);
					}
				}
			}
		}

		// 게임 상태 패킷 받기
		retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
		retval = recv(sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		// 패킷 타입 검사
		if (buf[0] == static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR)) {
			// 게임 클리어
			ChangeScene(SCENE_TYPE::CLEAR);
			IsGameClear = true;
			IsGameOver = false;
		}
		else if (buf[0] == static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER)) {
			// 게임 오버
			ChangeScene(SCENE_TYPE::GAMEOVER);
			IsGameOver = true;
			IsGameClear = false;
		}
	}
}

void CCore::progress()
{
	// 소켓이 연결되어 있으면 통신
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
	
	// 화면 Clear
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	// 그리기
	CSceneMgr::GetInst()->render(m_memDC);

	// 후면버퍼의 내용을 윈도우로 옮긴다.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY/* 그대로 복사 */);

	// 타이틀 바에 프레임 render
	CTimer::GetInst()->render();

	// 이벤트는 맨 마지막에 업데이트 (지연처리)
	CEventMgr::GetInst()->update();
}