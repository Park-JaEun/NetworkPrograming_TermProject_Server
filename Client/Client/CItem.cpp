#include "pch.h"
#include "CItem.h"
#include "CTexture.h"
#include "CResourceMgr.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CTimer.h"

CItem::CItem() : m_vFirstPos{Vec2(0.f, 0.f)}, m_fSpeed{50.f}, m_fMaxDistance{20.f}, m_bDir{true}
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Item", L"texture\\items\\items.bmp");
}

CItem::~CItem()
{
}

void CItem::update()
{
	Vec2 vCurPos = GetPos();

	if (m_bDir == true) {
		// 진행 방향으로 이동
		vCurPos.y += DT * m_fSpeed * 1;

		// 배회 거리 기준량을 넘었는지 확인
		float fDiff = abs(m_vFirstPos.y - vCurPos.y) - m_fMaxDistance;

		if (fDiff > 0.f) {
			// 방향 변경
			m_bDir = !m_bDir;
			// 넘어선 만큼 반대방향으로 이동
			vCurPos.y += fDiff * -1;
		}
	}

	if (m_bDir == false) {
		// 진행 방향으로 이동
		vCurPos.y += DT * m_fSpeed * -1;

		// 배회 거리 기준량을 넘었는지 확인
		float fDiff = abs(m_vFirstPos.y - vCurPos.y) - m_fMaxDistance;

		if (fDiff > 0.f) {
			// 방향 변경
			m_bDir = !m_bDir;
			// 넘어선 만큼 반대방향으로 이동
			vCurPos.y += fDiff * 1;
		}
	}

	SetPos(vCurPos);
}

void CItem::render(HDC _dc)
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Item", L"texture\\items\\items.bmp");
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vPos);

	if (GetName() == L"Bunny") {
		TransparentBlt(_dc,
			(int)vRenderPos.x,
			(int)vRenderPos.y,
			(int)GetScale().x,
			(int)GetScale().y,
			pTexture->GetDC(),
			0,
			0,
			25,
			25,
			RGB(255, 0, 255));
	}
	else if (GetName() == L"Cookie") {
		TransparentBlt(_dc,
			(int)vRenderPos.x,
			(int)vRenderPos.y,
			(int)GetScale().x,
			(int)GetScale().y,
			pTexture->GetDC(),
			25,
			0,
			25,
			25,
			RGB(255, 0, 255));
	}

	componentRender(_dc);
}


void CItem::OnCollision(CCollider* _pOther)
{
}

void CItem::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player") {
		// 플레이어와 충돌시 아이템을 삭제한다.
		DeleteObject(this);

		if (GetName() == L"Bunny")
			((CPlayer*)pOtherObj)->PlusBunnyCount();
		else if (GetName() == L"Cookie")
			((CPlayer*)pOtherObj)->PlusCookieCount();
	}
}

void CItem::ExitCollision(CCollider* _pOther)
{
}
