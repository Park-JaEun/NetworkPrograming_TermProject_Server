#include "pch.h"
#include "CCollider.h"
#include "CObject.h"


UINT CCollider::g_iNextID = 0;

CCollider::CCollider() : m_pObject(nullptr), m_vFinalPos{}, m_vOffsetPos{}, m_vScale{}, m_iID(g_iNextID++) /*ID�� ����*/, m_iCol(0)
{

}

CCollider::CCollider(const CCollider& _origin)
	: m_pObject(nullptr)
	, m_vFinalPos{}
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++) /*ID�� ����*/
	, m_iCol(0)
{
}

CCollider::~CCollider()
{

}

void CCollider::finalUpdate()
{
	// Object�� ��ġ�� ���󰣴�.
	m_vFinalPos = m_pObject->GetPos() + m_vOffsetPos;

	// �浹 ���ΰ� ������ �Ǹ� ����ó��
	assert(m_iCol >= 0);
}

bool CCollider::PtInCollider(Vec2 vPos)
{
	Vec2 vFinalPos = GetFinalPos();
	Vec2 vScale = GetScale();

	POINT ptMouse;
	ptMouse.x = (LONG)vPos.x;
	ptMouse.y = (LONG)vPos.y;

	// �浹ü�� �浹 ����
	RECT rcCollider{
		(int)(vFinalPos.x),
		(int)(vFinalPos.y),
		(int)(vFinalPos.x + vScale.x),
		(int)(vFinalPos.y + vScale.y) };

	// �浹ü�� �浹 ���� �ȿ� �ִ��� Ȯ��
	if (PtInRect(&rcCollider, ptMouse))
		return true;
	else
		return false;
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
