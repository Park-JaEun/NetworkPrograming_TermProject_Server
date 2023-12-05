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
	((CMonster*)pMonsterObj)->SetFirstPos(pMonsterObj->GetPos()); // SetFirstPos를 사용할 수 있도록 CMonster로 캐스팅

	// 충돌체, 애니메이터 생성
	pMonsterObj->CreateCollider();
	pMonsterObj->CreateAnimator();

	// 충돌체 크기 조정
	pMonsterObj->GetCollider()->SetScale(Vec2(30.f, 48.f));

	CreateObject(pMonsterObj, GROUP_TYPE::MONSTER);
}

void CreateFloor(const Vec2& _vPos)
{
	CObject* pFloorObj = new CFloor;

	pFloorObj->SetName(L"Floor");
	pFloorObj->SetPos(_vPos);

	// 애니메이터 생성
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

// 플레이어 ID를 설정하는 함수 
void setId(HWND hEditNickname, HWND hEditIP)
{
	int iLenNickname = GetWindowTextLength(hEditNickname);
	int iLenIP = GetWindowTextLength(hEditIP);
	int iPlayerID{};	// 플레이어 ID
	int retval{};		// 리턴값
	int size{};			// 수신할 데이터 크기
	char buf[BUFSIZE];	// 수신 버퍼

	TCHAR* pStrNickname = new TCHAR[iLenNickname + 1];
	TCHAR* pStrIP = new TCHAR[iLenIP + 1];

	// 서버의 IP와 나의 닉네임을 가져온다.
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

	// 서버에 접속
	SOCKET sock = CCore::GetInst()->GetSocket();

	// 소켓 생성 및 할당
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");
	CCore::GetInst()->SetSocket(sock);

	// 서버 주소 설정
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, pStrIPChar, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		err_quit("connect()");

	// 닉네임 송신
	retval = send(sock, reinterpret_cast<char*>(&iLenNickname), sizeof(int), 0);
	retval = send(sock, (char*)pStrNicknameChar, iLenNickname, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		closesocket(sock);
		WSACleanup();
		// 오류시 강제종료
		PostQuitMessage(0);
	}
	std::cout << "닉네임 송신" << std::endl;

	// 플레이어 ID 수신
	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		PostQuitMessage(0);
	}

	SC_MAKE_ID_PACKET* IDPacket = reinterpret_cast<SC_MAKE_ID_PACKET*>(buf);
	std::cout << "Player" << IDPacket->id << " 연결 성공" << std::endl;

	// 플레이어 ID 저장
	CCore::GetInst()->SetID(IDPacket->id);

	// 플레이어 닉네임 저장
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
	std::cout << "캐릭터 선택 패킷 송신" << std::endl;
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
	std::cout << "스테이지 초기화 패킷 수신" << std::endl;
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
}


void recvMonsterInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

	// 몬스터 정보 받기
	SC_MONSTER_PACKET* pMonsterPacket = reinterpret_cast<SC_MONSTER_PACKET*>(buf);

	// 몬스터 수 받기
	int monsterCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		exit(1);
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
}


void recvBossInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

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


void recvRabbitItemInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

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
}

void recvCookieInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

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
}


void recvPlayerBulletInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

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
}

void recvMonsterBulletInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

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
}


void recvBossBulletInfo(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

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
}


void recvGameStateSignal(SOCKET sock)
{
	int retval{};
	int size{};
	char buf[BUFSIZE]{};

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
		CCore::GetInst()->SetGameClear(true);
		CCore::GetInst()->SetGameOver(false);
	}
	else if (buf[0] == static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER)) {
		// 게임 오버
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
	std::cout << "초기화 완료 패킷 송신" << std::endl;

	std::cout << "게임 시작 패킷 수신 대기중" << std::endl;
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

