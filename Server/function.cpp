#include "pch.h"
#include "function.h"
#include "CObject.h"
#include "CMonster.h"
#include "CCollider.h"
#include "CEventMgr.h"
#include "CObjectMgr.h"

std::mutex g_mutex_f;



void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent event{};
	event.eEvent = EVENT_TYPE::CREATE_OBJECT;
	event.lParam = (DWORD_PTR)_pObj;
	event.wParam = (DWORD_PTR)_eGroup;

	ADDEVENT(event);
}

void CreateBoss()
{
	// 보스 오브젝트 만들기
	CObject* pBossObj = new CBoss;

	pBossObj->SetName(L"Boss");
	pBossObj->SetPos(Vec2(5250.f, 360.f));

	// 충돌체, 애니메이터 생성
	pBossObj->CreateCollider();

	pBossObj->GetCollider()->SetScale(Vec2(33.f * 2.f, 64.f * 4.6f));

	CreateObject(pBossObj, GROUP_TYPE::BOSS);
}

void CreateMonster(const Vec2& _vPos, int id)
{
	CObject* pMonsterObj = new CMonster;

	pMonsterObj->SetName(L"Monster" + std::to_wstring(id));
	pMonsterObj->SetPos(_vPos);
	((CMonster*)pMonsterObj)->SetGroupType(GROUP_TYPE::MONSTER);
	((CMonster*)pMonsterObj)->SetID(id);
	((CMonster*)pMonsterObj)->SetFirstPos(pMonsterObj->GetPos()); // SetFirstPos를 사용할 수 있도록 CMonster로 캐스팅

	// 충돌체 생성
	pMonsterObj->CreateCollider();

	// 충돌체 크기 조정
	pMonsterObj->GetCollider()->SetScale(Vec2(30.f, 48.f));

	CreateObject(pMonsterObj, GROUP_TYPE::MONSTER);
}

void CreateMonsters()
{
	// 몬스터 오브젝트들 만들기
	CreateMonster(Vec2(800.f, -90.f), 0);
	CreateMonster(Vec2(910.f, 50.f), 1);
	CreateMonster(Vec2(1440.f, -150.f), 2);
	CreateMonster(Vec2(1440.f, 50.f), 3);
	CreateMonster(Vec2(1440.f, -50.f), 4);
	CreateMonster(Vec2(2010.f, -150.f), 5);
	CreateMonster(Vec2(2130.f, 50.f), 6);
	CreateMonster(Vec2(2250.f, -50.f), 7);
	CreateMonster(Vec2(2770.f, 50.f), 8);
	CreateMonster(Vec2(2890.f, -50.f), 9);
	CreateMonster(Vec2(3010.f, -150.f), 10);
	CreateMonster(Vec2(3460.f, -150.f), 11);
	CreateMonster(Vec2(3660.f, -50.f), 12);
	CreateMonster(Vec2(3660.f, 70.f), 13);
	CreateMonster(Vec2(3840.f, -150.f), 14);
	CreateMonster(Vec2(4340.f, -50.f), 15);
	CreateMonster(Vec2(4200.f, -150.f), 16);
	CreateMonster(Vec2(4200.f, 50.f), 17);
	CreateMonster(Vec2(4480.f, -150.f), 18);
	CreateMonster(Vec2(4480.f, 50.f), 19);
}

void CreateBunny(const Vec2& _vPos, int id)
{
	CItem* pBunnyItemObj = new CItem;

	pBunnyItemObj->SetName(L"Bunny" + std::to_wstring(id));
	pBunnyItemObj->SetID(id);
	pBunnyItemObj->SetPos(_vPos);
	pBunnyItemObj->SetFirstPos(_vPos);
	pBunnyItemObj->SetScale(Vec2(25.f, 25.f));
	pBunnyItemObj->SetGroupType(GROUP_TYPE::ITEM_RABBIT);

	pBunnyItemObj->CreateCollider();
	pBunnyItemObj->GetCollider()->SetScale(Vec2(25.f, 25.f));
	pBunnyItemObj->GetCollider()->SetOffsetPos(Vec2(12.5f, 12.5f));

	CreateObject(pBunnyItemObj, GROUP_TYPE::ITEM_RABBIT);
}

void CreateCookie(const Vec2& _vPos, int id)
{
	CItem* pCookieItemObj = new CItem;

	pCookieItemObj->SetName(L"Cookie" + std::to_wstring(id));
	pCookieItemObj->SetID(id);
	pCookieItemObj->SetPos(_vPos);
	pCookieItemObj->SetFirstPos(_vPos);
	pCookieItemObj->SetScale(Vec2(25.f, 25.f));
	pCookieItemObj->SetGroupType(GROUP_TYPE::ITEM_COOKIE);

	pCookieItemObj->CreateCollider();
	pCookieItemObj->GetCollider()->SetScale(Vec2(25.f, 25.f));
	pCookieItemObj->GetCollider()->SetOffsetPos(Vec2(12.5f, 12.5f));

	CreateObject(pCookieItemObj, GROUP_TYPE::ITEM_COOKIE);
}

void CreateItems()
{
	// 아이템 오브젝트들 만들기
	CreateBunny(Vec2(710.f, -83.f - 25.f), 0);
	CreateBunny(Vec2(1350.f, -140.f - 25.f), 1);
	CreateBunny(Vec2(1355.f, 60.f - 25.f), 2);
	CreateBunny(Vec2(2165.f, -40.f - 25.f), 3);
	CreateBunny(Vec2(2855.f, 60.f - 25.f), 4);
	CreateBunny(Vec2(3375.f, -140.f - 25.f), 5);
	CreateBunny(Vec2(3925.f, -140.f - 25.f), 6);
	CreateBunny(Vec2(3660.f, 80.f - 25.f), 7);
	CreateBunny(Vec2(4195.f, 60.f - 25.f), 8);
	CreateBunny(Vec2(4480.f, 60.f - 25.f), 9);
	CreateBunny(Vec2(4340.f, -40.f - 25.f), 10);
	CreateBunny(Vec2(4480.f, -140.f - 25.f), 11);
	CreateBunny(Vec2(4195.f, -140.f - 25.f), 12);

	CreateCookie(Vec2(880.f, -80.f - 25.f), 0);
	CreateCookie(Vec2(760.f, 100.f - 25.f), 1);
	CreateCookie(Vec2(1525.f, -40.f - 25.f), 2);
	CreateCookie(Vec2(2095.f, -150.f - 25.f), 3);
	CreateCookie(Vec2(2215.f, 60.f - 25.f), 4);
	CreateCookie(Vec2(2975.f, -40.f - 25.f), 5);
	CreateCookie(Vec2(3093.f, -140.f - 25.f), 6);
	CreateCookie(Vec2(3660.f, -40.f - 25.f), 7);
	CreateCookie(Vec2(5065.f, 0.f - 25.f), 8);
}

void DeleteObject(CObject* _pObj)
{
	tEvent event{};
	event.eEvent = EVENT_TYPE::DELETE_OBJECT;
	event.lParam = (DWORD_PTR)_pObj;
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

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

void sendPlayerId(int retval, SOCKET sock, int id, int size)
{

	// 클라이언트 정보 송신
	SC_MAKE_ID_PACKET idPacket;
	idPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	idPacket.id = id;
	size = sizeof(SC_MAKE_ID_PACKET);

	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&idPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}

	std::cout << "[Player" << idPacket.id << "] ID 할당 패킷 전송" << std::endl;
}

int recvSelectCharacter(int retval, SOCKET sock, int size, char* buf)
{
	retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;
}

int sendInitSignal(int retval, int size, SOCKET sock)
{
	SC_INIT_PACKET initPacket;
	size = sizeof(SC_INIT_PACKET);
	initPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);

	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&initPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "모든 플레이어 준비 완료" << std::endl;
	std::cout << "플레이어들에게 초기화 신호 패킷 전송" << std::endl;

}

int recvInitFinishSignal(int retval, int size, SOCKET sock)
{
	SC_GAME_START_PACKET startPacket;
	startPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
	size = sizeof(SC_GAME_START_PACKET);

	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&startPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

}

void sendMonsterInfo(SOCKET sock, int size, int retval)
{
	std::vector<CObject*> vecMonster;
	{
		std::lock_guard<std::mutex> lock{ g_mutex_f };
		vecMonster = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::MONSTER);

	}

	SC_MONSTER_PACKET monsterPacket;
	monsterPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
	size = sizeof(SC_MONSTER_PACKET);

	// 몬스터 수 전송
	int monsterCount = vecMonster.size();
	retval = send(sock, reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return ;
	}

	// 몬스터 정보 보내기
	for (CObject* pMonster : vecMonster) {
		monsterPacket.monsterID = ((CMonster*)pMonster)->GetID();
		monsterPacket.monsterPos = pMonster->GetPos();
		monsterPacket.monsterState = ((CMonster*)pMonster)->GetState();
		monsterPacket.monsterDir = ((CMonster*)pMonster)->GetDir();
		monsterPacket.monsterIsDead = pMonster->IsDead();

		retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(sock, reinterpret_cast<char*>(&monsterPacket), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
	}
}

void sendBossInfo(SOCKET sock, int size, int retval, bool& isBoss, bool& IsGameClear)
{

	std::vector<CObject*> vecBoss;
	// 모든 플레이어의 x좌표 위치가 5060.f 이상이면 isBoss를 true로 변경
	{
		std::lock_guard<std::mutex> lock{ g_mutex_f };
		vecBoss = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BOSS);
	}

	CObject* pBoss = CObjectMgr::GetInst()->FindObject(L"Boss");

	if (!isBoss) {
		for (int i = 0; i < MAX_PLAYER; ++i) {
			CObject* pPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(i));

			if (pPlayer->GetPos().x < 5060.f) {
				isBoss = false;
				break;
			}
			else
				isBoss = true;
		}

		if (isBoss) {
			// 보스 출현
			((CBoss*)pBoss)->SetHaveToAppear(true);
			((CBoss*)pBoss)->SetState(BOSS_STATE::IDLE);
			std::cout << "보스 출현" << std::endl;
		}
		else {
			// 보스 수 보내기
			int bossCount = vecBoss.size();
			retval = send(sock, reinterpret_cast<char*>(&bossCount), sizeof(bossCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				return ;
			}

			// 보스 정보 보내기
			SC_BOSS_PACKET bossPacket;
			bossPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
			size = sizeof(SC_BOSS_PACKET);

			bossPacket.bossPos = Vec2(5250.f, 380.f);
			bossPacket.bossState = BOSS_STATE::NOT_APPEAR;
			bossPacket.bossIsDead = false;

			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&bossPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				return ;
			}
		}
	}

	// 보스 정보 송신 (위치, 상태)
	if (isBoss) {
		// 보스 수 보내기
		int bossCount = vecBoss.size();
		retval = send(sock, reinterpret_cast<char*>(&bossCount), sizeof(bossCount), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return ;
		}

		// 보스 정보 보내기
		SC_BOSS_PACKET bossPacket;
		bossPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
		size = sizeof(SC_BOSS_PACKET);

		for (CObject* pBoss : vecBoss) {
			bossPacket.bossPos = pBoss->GetPos();
			bossPacket.bossState = ((CBoss*)pBoss)->GetState();
			bossPacket.bossIsDead = pBoss->IsDead();

			// 보스가 죽었으면 게임 클리어
			if (pBoss->IsDead()) {
				{
					std::lock_guard<std::mutex> lock{ g_mutex_f };
					IsGameClear = true;
				}
			}

			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&bossPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
	}
}

void sendRabbitInfo(SOCKET sock, int size, int retval)
{

	// 공유 자원 접근 동기화
	std::vector<CObject*> copiedVecRabbitItem;
	{
		std::lock_guard<std::mutex> lock{ g_mutex_f };
		copiedVecRabbitItem = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::ITEM_RABBIT);
	}

	// 토끼 아이템 수 전송
	int rabbitItemCount = copiedVecRabbitItem.size();
	retval = send(sock, reinterpret_cast<char*>(&rabbitItemCount), sizeof(rabbitItemCount), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}

	// 토끼 아이템 정보 보내기
	for (CObject* pRabbitItem : copiedVecRabbitItem) {
		SC_RABBIT_ITEM_PACKET rabbitItemPacket;
		rabbitItemPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_RABBIT_ITEM);
		size = sizeof(SC_RABBIT_ITEM_PACKET);

		rabbitItemPacket.itemID = ((CItem*)pRabbitItem)->GetID();
		rabbitItemPacket.itemPos = ((CItem*)pRabbitItem)->GetPos();
		rabbitItemPacket.itemIsDead = pRabbitItem->IsDead();

		retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(sock, reinterpret_cast<char*>(&rabbitItemPacket), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
	}
}

void sendCookieInfo(SOCKET sock, int size, int retval)
{
	std::vector<CObject*> copiedVecCookieItem;

	{
		std::lock_guard<std::mutex> lock{ g_mutex_f };
		copiedVecCookieItem = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::ITEM_COOKIE); // 공유 자원 복사
	}

	// 쿠키 아이템 수 전송
	int cookieItemCount = copiedVecCookieItem.size();
	retval = send(sock, reinterpret_cast<char*>(&cookieItemCount), sizeof(cookieItemCount), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}

	// 쿠키 아이템 정보 보내기
	for (CObject* pCookieItem : copiedVecCookieItem) {
		SC_COOKIE_ITEM_PACKET cookieItemPacket;
		cookieItemPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_COOKIE_ITEM);
		size = sizeof(SC_COOKIE_ITEM_PACKET);

		cookieItemPacket.itemID = ((CItem*)pCookieItem)->GetID();
		cookieItemPacket.itemPos = ((CItem*)pCookieItem)->GetPos();
		cookieItemPacket.itemIsDead = pCookieItem->IsDead();

		retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(sock, reinterpret_cast<char*>(&cookieItemPacket), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
	}

}

void sendPlayerCopiedInfo(SOCKET sock, int size, int retval)
{
	std::vector<CObject*> copiedVecBullet;

	// 락 사용으로 공유 자원 복사
	{
		std::lock_guard<std::mutex> lock{ g_mutex_f };
		copiedVecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_PLAYER);
	}

	// 클라이언트에게 플레이어 투사체 수 전송
	int bulletCount = copiedVecBullet.size();
	retval = send(sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return ;
	}

	if (bulletCount != 0) {
		// 클라이언트에 플레이어 투사체 정보 보내기
		for (CObject* pBullet : copiedVecBullet) {
			SC_BULLET_PACKET bulletPacket;
			bulletPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(SC_BULLET_PACKET);

			bulletPacket.playerID = ((CBullet*)pBullet)->GetPlayerID();
			bulletPacket.bulletID = ((CBullet*)pBullet)->GetID();
			bulletPacket.bulletPos = ((CBullet*)pBullet)->GetPos();
			bulletPacket.bulletIsDead = ((CBullet*)pBullet)->IsDead();
			bulletPacket.bulletDir = ((CBullet*)pBullet)->GetDir();
			bulletPacket.bulletDegree = 0.f;

			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&bulletPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
	}


}

void sendMonsterCopiedInfo(SOCKET sock, int size, int retval)
{
	std::vector<CObject*> copiedVecBullet;

	// 락을 사용으로 공유 자원 복사
	{
		std::lock_guard<std::mutex> lock{ g_mutex_f };
		copiedVecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_MONSTER); // 공유 자원 복사
	}

	// 클라이언트에게 몬스터 투사체 수 전송
	int bulletCount = copiedVecBullet.size();
	retval = send(sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}

	if (bulletCount != 0) {
		// 클라이언트에 몬스터 투사체 정보 보내기
		for (CObject* pBullet : copiedVecBullet) {
			SC_BULLET_PACKET bulletPacket;
			bulletPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(SC_BULLET_PACKET);

			bulletPacket.playerID = ((CBullet*)pBullet)->GetPlayerID();
			bulletPacket.bulletID = ((CBullet*)pBullet)->GetID();
			bulletPacket.bulletPos = ((CBullet*)pBullet)->GetPos();
			bulletPacket.bulletIsDead = ((CBullet*)pBullet)->IsDead();
			bulletPacket.bulletDir = ((CBullet*)pBullet)->GetDir();
			bulletPacket.bulletDegree = 0.f;

			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&bulletPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
	}
}

void sendBossCopiedInfo(SOCKET sock, int size, int retval)
{
	std::vector<CObject*> copiedVecBullet;

	// 락을 사용하여 공유 자원에 대한 접근을 동기화
	{
		std::lock_guard<std::mutex> lock{ g_mutex_f };
		copiedVecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_BOSS); // 공유 자원 복사
	}

	// 클라이언트에게 보스 투사체 수 전송
	int bulletCount = copiedVecBullet.size();
	retval = send(sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}

	if (bulletCount != 0) {
		// 클라이언트에게 보스 투사체 정보 보내기
		for (CObject* pBullet : copiedVecBullet) {
			SC_BULLET_PACKET bulletPacket;
			bulletPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(SC_BULLET_PACKET);

			bulletPacket.playerID = ((CBullet*)pBullet)->GetPlayerID();
			bulletPacket.bulletID = ((CBullet*)pBullet)->GetID();
			bulletPacket.bulletPos = ((CBullet*)pBullet)->GetPos();
			bulletPacket.bulletIsDead = ((CBullet*)pBullet)->IsDead();
			bulletPacket.bulletDir = ((CBullet*)pBullet)->GetDir();
			bulletPacket.bulletDegree = ((CBullet*)pBullet)->GetDegree();

			retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(sock, reinterpret_cast<char*>(&bulletPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
	}

}

void sendRankInfo(SOCKET sock, int retval, int size, int id)
{

	SC_GAME_CLEAR_PACKET clearPacket;
	clearPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
	size = sizeof(SC_GAME_CLEAR_PACKET);

	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&clearPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}
	std::cout << id << "번 플레이어 게임 클리어 패킷 전송" << std::endl;

	SC_RANK_PACKET rankPacket;
	rankPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_RANK);
	size = sizeof(SC_RANK_PACKET);

	// 패킷 초기화
	for (int i = 0; i < MAX_PLAYER; ++i) {
		rankPacket.id[i] = 0;
		rankPacket.score[i] = 0;
	}

	// 클라이언트들 스코어 합산
	for (int i = 0; i < MAX_PLAYER; ++i) {
		CObject* pPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(i));

		rankPacket.id[i] = i;
		rankPacket.score[i] = ((CPlayer*)pPlayer)->GetKillCount() * 100 + ((CPlayer*)pPlayer)->GetCookieCount() * 30 + ((CPlayer*)pPlayer)->GetBunnyCount() * 50.f;
		rankPacket.character[i] = ((CPlayer*)pPlayer)->GetType();
	}

	// 스코어 정보 전송
	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&rankPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}
}

int recvSignal(SOCKET sock, char* buf, int retval, int size, bool& isLobby, bool& IsExit)
{
	memset(buf, 0, BUFSIZE);

	// 데이터 받기
	retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;

	if (buf[0] == static_cast<char>(CS_PACKET_TYPE::CS_SELECT_LOBBY)) {
		isLobby = true;
		IsExit = false;
		return 0;
	}

	if (buf[0] == static_cast<char>(CS_PACKET_TYPE::CS_SELECT_EXIT)) {
		IsExit = true;
		isLobby = false;
		return 0;
	}


}



