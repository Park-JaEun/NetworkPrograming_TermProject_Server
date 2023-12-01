#include "pch.h"
#include "CItem.h"
#include "CCollider.h"
#include "CTimer.h"


CItem::CItem() : m_vFirstPos{ Vec2(0.f, 0.f) }, m_fSpeed{ 50.f }, m_fMaxDistance{ 20.f }, m_bDir{ true }
{
}

CItem::~CItem()
{
}

void CItem::update()
{
	Vec2 vCurPos = GetPos();

	if (m_bDir == true) {
		// ���� �������� �̵�
		vCurPos.y += DT * m_fSpeed * 1;

		// ��ȸ �Ÿ� ���ط��� �Ѿ����� Ȯ��
		float fDiff = abs(m_vFirstPos.y - vCurPos.y) - m_fMaxDistance;

		if (fDiff > 0.f) {
			// ���� ����
			m_bDir = !m_bDir;
			// �Ѿ ��ŭ �ݴ�������� �̵�
			vCurPos.y += fDiff * -1;
		}
	}

	if (m_bDir == false) {
		// ���� �������� �̵�
		vCurPos.y += DT * m_fSpeed * -1;

		// ��ȸ �Ÿ� ���ط��� �Ѿ����� Ȯ��
		float fDiff = abs(m_vFirstPos.y - vCurPos.y) - m_fMaxDistance;

		if (fDiff > 0.f) {
			// ���� ����
			m_bDir = !m_bDir;
			// �Ѿ ��ŭ �ݴ�������� �̵�
			vCurPos.y += fDiff * 1;
		}
	}

	SetPos(vCurPos);
}

void CItem::OnCollision(CCollider* _pOther)
{
}

void CItem::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (pOtherObj->GetName() == L"Player" + std::to_wstring(i)) {
			DeleteObject(this);

			if (GetName() == L"Bunny")
				((CPlayer*)pOtherObj)->PlusBunnyCount();
			else if (GetName() == L"Cookie")
				((CPlayer*)pOtherObj)->PlusCookieCount();
		}
	}
}

void CItem::ExitCollision(CCollider* _pOther)
{
}
