#include "pch.h"
#include "CObject.h"

#include "CKeyMgr.h"
#include "CTimer.h"

#include "CCollider.h"

CObject::CObject() : m_strName{}, m_vPos{}, m_vScale{}, m_pCollider(nullptr), m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)
	: m_strName{ _origin.m_strName }
	, m_vPos{ _origin.m_vPos }
	, m_vScale{ _origin.m_vScale }
	, m_pCollider(nullptr)
	, m_bAlive(true)
{
	if (_origin.m_pCollider != nullptr) {
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->SetObj(this);
		m_pCollider->SetScale(this->GetScale());
	}
}

CObject::~CObject()
{
	if (m_pCollider != nullptr)
		delete m_pCollider;
}

void CObject::finalUpdate()
{
	if (m_pCollider != nullptr)
		m_pCollider->finalUpdate();
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->SetObj(this);				// 오브젝트 할당
	m_pCollider->SetScale(this->GetScale());	// 기본 크기 설정
}
