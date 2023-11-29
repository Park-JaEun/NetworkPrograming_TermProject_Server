#include "pch.h"
#include "function.h"
#include "CObject.h"
#include "CMonster.h"
#include "CCollider.h"
#include "CEventMgr.h"

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

void CreateMonster(const Vec2& _vPos)
{
	CObject* pMonsterObj = new CMonster;

	pMonsterObj->SetName(L"Monster");
	pMonsterObj->SetPos(_vPos);
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
	CreateMonster(Vec2(800.f, -90.f));
	CreateMonster(Vec2(910.f, 50.f));
	CreateMonster(Vec2(1440.f, -150.f));
	CreateMonster(Vec2(1440.f, 50.f));
	CreateMonster(Vec2(1440.f, -50.f));
	CreateMonster(Vec2(2010.f, -150.f));
	CreateMonster(Vec2(2130.f, 50.f));
	CreateMonster(Vec2(2250.f, -50.f));
	CreateMonster(Vec2(2770.f, 50.f));
	CreateMonster(Vec2(2890.f, -50.f));
	CreateMonster(Vec2(3010.f, -150.f));
	CreateMonster(Vec2(3460.f, -150.f));
	CreateMonster(Vec2(3660.f, -50.f));
	CreateMonster(Vec2(3660.f, 70.f));
	CreateMonster(Vec2(3840.f, -150.f));
	CreateMonster(Vec2(4340.f, -50.f));
	CreateMonster(Vec2(4200.f, -150.f));
	CreateMonster(Vec2(4200.f, 50.f));
	CreateMonster(Vec2(4480.f, -150.f));
	CreateMonster(Vec2(4480.f, 50.f));
}

void CreateBunny(const Vec2& _vPos)
{
	CItem* pBunnyItemObj = new CItem;

	pBunnyItemObj->SetName(L"Bunny");
	pBunnyItemObj->SetPos(_vPos);
	pBunnyItemObj->SetFirstPos(_vPos);
	pBunnyItemObj->SetScale(Vec2(25.f, 25.f));

	pBunnyItemObj->CreateCollider();
	pBunnyItemObj->GetCollider()->SetScale(Vec2(25.f, 25.f));
	pBunnyItemObj->GetCollider()->SetOffsetPos(Vec2(12.5f, 12.5f));

	CreateObject(pBunnyItemObj, GROUP_TYPE::ITEM);
}

void CreateCookie(const Vec2& _vPos)
{
	CItem* pCookieItemObj = new CItem;

	pCookieItemObj->SetName(L"Cookie");
	pCookieItemObj->SetPos(_vPos);
	pCookieItemObj->SetFirstPos(_vPos);
	pCookieItemObj->SetScale(Vec2(25.f, 25.f));

	pCookieItemObj->CreateCollider();
	pCookieItemObj->GetCollider()->SetScale(Vec2(25.f, 25.f));
	pCookieItemObj->GetCollider()->SetOffsetPos(Vec2(12.5f, 12.5f));

	CreateObject(pCookieItemObj, GROUP_TYPE::ITEM);
}

void CreateItems()
{
	// 아이템 오브젝트들 만들기
	CreateBunny(Vec2(710.f, -83.f - 25.f));
	CreateBunny(Vec2(1350.f, -140.f - 25.f));
	CreateBunny(Vec2(1355.f, 60.f - 25.f));
	CreateBunny(Vec2(2165.f, -40.f - 25.f));
	CreateBunny(Vec2(2855.f, 60.f - 25.f));
	CreateBunny(Vec2(3375.f, -140.f - 25.f));
	CreateBunny(Vec2(3925.f, -140.f - 25.f));
	CreateBunny(Vec2(3660.f, 80.f - 25.f));
	CreateBunny(Vec2(4195.f, 60.f - 25.f));
	CreateBunny(Vec2(4480.f, 60.f - 25.f));
	CreateBunny(Vec2(4340.f, -40.f - 25.f));
	CreateBunny(Vec2(4480.f, -140.f - 25.f));
	CreateBunny(Vec2(4195.f, -140.f - 25.f));

	CreateCookie(Vec2(880.f, -80.f - 25.f));
	CreateCookie(Vec2(760.f, 100.f - 25.f));
	CreateCookie(Vec2(1525.f, -40.f - 25.f));
	CreateCookie(Vec2(2095.f, -150.f - 25.f));
	CreateCookie(Vec2(2215.f, 60.f - 25.f));
	CreateCookie(Vec2(2975.f, -40.f - 25.f));
	CreateCookie(Vec2(3093.f, -140.f - 25.f));
	CreateCookie(Vec2(3660.f, -40.f - 25.f));
	CreateCookie(Vec2(5065.f, 0.f - 25.f));
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