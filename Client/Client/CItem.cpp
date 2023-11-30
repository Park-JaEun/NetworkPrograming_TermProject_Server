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
}

void CItem::ExitCollision(CCollider* _pOther)
{
}
