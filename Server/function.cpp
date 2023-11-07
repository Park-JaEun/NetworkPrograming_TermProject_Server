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

// 원래는 플레이어 카메라 밖으로 나가면 사라짐, 멀티 게임으로 만들기 위해 최대 사거리에 도달하면 삭제하도록 한다.
//bool IsInCamera(const Vec2& _vPos)
//{
//	Vec2 vResolution = CCore::GetInst()->GetResolution();
//	Vec2 vCameraLookAt = CCamera::GetInst()->GetLookAt();
//
//	if (_vPos.y < vCameraLookAt.y + (vResolution.y / 2.f) &&
//		_vPos.y > vCameraLookAt.y - (vResolution.y / 2.f) &&
//		_vPos.x < vCameraLookAt.x + (vResolution.x / 2.f) &&
//		_vPos.x > vCameraLookAt.x - (vResolution.x / 2.f))
//		return true;
//	return false;
//}
