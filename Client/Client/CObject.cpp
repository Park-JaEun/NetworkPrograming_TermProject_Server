#include "pch.h"
#include "CObject.h"

#include "CKeyMgr.h"
#include "CTimer.h"

#include "CCollider.h"
#include "CAnimator.h"

CObject::CObject() : m_strName{}, m_vPos{}, m_vScale{}, m_pCollider(nullptr), m_pAnimator(nullptr), m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)
	: m_strName{ _origin.m_strName }
	, m_vPos{ _origin.m_vPos }
	, m_vScale{ _origin.m_vScale }
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_bAlive(true)
{
	if (_origin.m_pCollider != nullptr) {
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->SetObj(this);
		m_pCollider->SetScale(this->GetScale());
	}

	if (_origin.m_pAnimator != nullptr) {
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->SetObj(this);
	}
}

CObject::~CObject()
{
	if (m_pCollider != nullptr)
		delete m_pCollider;
	if (m_pAnimator != nullptr)
		delete m_pAnimator;
}

void CObject::finalUpdate()
{
	if (m_pCollider != nullptr)
		m_pCollider->finalUpdate();
}

void CObject::render(HDC _dc)
{
	// 렌더링 좌표로 변환
	/*Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	Rectangle(_dc,
		(int)(vRenderPos.x - m_vScale.x / 2.f),
		(int)(vRenderPos.y - m_vScale.y / 2.f),
		(int)(vRenderPos.x + m_vScale.x / 2.f),
		(int)(vRenderPos.y + m_vScale.y / 2.f));

	componentRender(_dc);*/
}

void CObject::componentRender(HDC _dc)
{
	if (m_pCollider != nullptr)
		m_pCollider->render(_dc);

	if (m_pAnimator != nullptr)
		m_pAnimator->render(_dc);
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->SetObj(this);				// 오브젝트 할당
	m_pCollider->SetScale(this->GetScale());	// 기본 크기 설정
}
