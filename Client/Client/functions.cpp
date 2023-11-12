#include "pch.h"
#include "functions.h"

#include "CObject.h"
#include "CFloor.h"
#include "CMonster.h"

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

void CreateMonster(const Vec2& _vPos)
{
	CObject* pMonsterObj = new CMonster;

	pMonsterObj->SetName(L"Monster");
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
