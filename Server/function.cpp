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