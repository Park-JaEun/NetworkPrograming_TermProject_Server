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

Vec2 Lerp(Vec2 start, Vec2 end, float alpha) {
	return start + (end - start) * alpha;
}