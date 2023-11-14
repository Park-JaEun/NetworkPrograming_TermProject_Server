#include "pch.h"
#include "CCollider.h"
#include "CObject.h"


UINT CCollider::g_iNextID = 0;

CCollider::CCollider() : m_pObject(nullptr), m_vFinalPos{}, m_vOffsetPos{}, m_vScale{}, m_iID(g_iNextID++) /*ID값 갱신*/, m_iCol(0)
{

}

CCollider::CCollider(const CCollider& _origin)
	: m_pObject(nullptr)
	, m_vFinalPos{}
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++) /*ID값 갱신*/
	, m_iCol(0)
{
}

CCollider::~CCollider()
{

}

void CCollider::finalUpdate()
{
	// Object의 위치를 따라간다.
	m_vFinalPos = m_pObject->GetPos() + m_vOffsetPos;

	// 충돌 여부가 음수가 되면 오류처리
	assert(m_iCol >= 0);
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pObject->OnCollision(_pOther);
}

void CCollider::EnterCollision(CCollider* _pOther)
{
	++m_iCol;
	m_pObject->EnterCollision(_pOther);
}

void CCollider::ExitCollision(CCollider* _pOther)
{
	--m_iCol;
	m_pObject->ExitCollision(_pOther);
}
