#include "pch.h"
#include "CItem.h"
#include "CTexture.h"
#include "CResourceMgr.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CTimer.h"

CItem::CItem() : m_vFirstPos{Vec2(0.f, 0.f)}, m_fSpeed{50.f}, m_fMaxDistance{20.f}, 
				 m_bDir{true}, m_eType{ITEM_TYPE::BUNNY}, m_vPrevPos{Vec2(0.f, 0.f)}, m_fTime{0.f}
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Item", L"texture\\items\\items.bmp");
}

CItem::~CItem()
{
	/*if (GetType() == ITEM_TYPE::BUNNY) {
		std::cout << "Bunny Item Destroy" << std::endl;
	}
	
	if (GetType() == ITEM_TYPE::COOKIE) {
		std::cout << "Cookie Item Destroy" << std::endl;
	}*/
}

void CItem::update()
{
	PredictItemPos();	// 예측
	InterpolatePos();	// 보간

	// 이전 위치와 현재 위치가 변화가 없는 시간이 0.5초가 넘으면 존재하지 않는다고 판단
	if ((int)m_vPrevPos.y == (int)GetPos().y) {
		m_fTime += DT;
		if (m_fTime > 0.5f) {
			DeleteObject(this);
		}
	}
	else {
		m_fTime = 0.f;
		m_vPrevPos = GetPos();
	}
}

void CItem::render(HDC _dc)
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Item", L"texture\\items\\items.bmp");
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vPos);

	if (GetType() == ITEM_TYPE::BUNNY) {
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
	else if (GetType() == ITEM_TYPE::COOKIE) {
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


void CItem::PredictItemPos()
{
	// 마지막 방향을 사용하여 보스의 위치 예측
	Vec2 vPos = GetPos();
	float fInterforate = 0.5f;

	// 전 y좌표보다 현재 y좌표가 더 크다면
	if (vPos.y > m_vPrevPos.y)
	{
		vPos.y += (m_fSpeed / fInterforate) * DT;
	}
	else if (vPos.y < m_vPrevPos.y)
	{
		vPos.y -= (m_fSpeed / fInterforate) * DT;
	}

	SetPos(vPos);
}

void CItem::InterpolatePos()
{
	// 보간 로직
	// 이전 포지션과 현재 포지션 상태 사이를 보간
	Vec2 vPos = GetPos();
	Vec2 vPrevPos = m_vPrevPos;
	Vec2 interpolatePos{};

	// 보간 정도
	// 0 ~ 1 사이의 값
	float fInterpolate = 0.5f;

	interpolatePos.x = LerpX(vPrevPos.x, vPos.x, fInterpolate);
	interpolatePos.y = LerpY(vPrevPos.y, vPos.y, fInterpolate);

	SetPos(interpolatePos);
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
