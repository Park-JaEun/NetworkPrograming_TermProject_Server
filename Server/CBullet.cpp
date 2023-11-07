#include "pch.h"
#include "CBullet.h"

#include "CTimer.h"
#include "CCollider.h"

CBullet::CBullet() : m_fDir(1.f), m_fSpeed(200.f), m_bIsDown(false), m_fDegree(0.f)
{
}

CBullet::~CBullet()
{
}

void CBullet::update()
{
	Vec2 vPos = GetPos();

	if (m_fDegree != 0.f) {
		vPos.x += DT * m_fSpeed * cosf(m_fDegree);
		vPos.y += DT * m_fSpeed * sinf(m_fDegree);
	}
	else {
		if (m_bIsDown)
			vPos.y += DT * m_fSpeed * m_fDir;
		else
			vPos.x += DT * m_fSpeed * m_fDir;
	}

	SetPos(vPos);

	// 카메라나 게임월드 밖으로 나가면 삭제 이벤트 발생
	if (!IsInWorld(vPos) || !IsInCamera(vPos))
		DeleteObject(this);
}

void CBullet::SetDir(int _iDir)
{
	if (_iDir == DIR_RIGHT)
		m_fDir = 1.f;
	else if (_iDir == DIR_LEFT)
		m_fDir = -1.f;
	else if (_iDir == DIR_DOWN) {
		m_fDir = 1.f;
		m_bIsDown = true;
	}
}

void CBullet::OnCollision(CCollider* _pOther)
{
}

void CBullet::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster") {
		DeleteObject(this);
	}

	if (pOtherObj->GetName() == L"Boss") {
		DeleteObject(this);
	}

	if (pOtherObj->GetName() == L"Player") {
		DeleteObject(this);
	}
}

void CBullet::ExitCollision(CCollider* _pOther)
{
}