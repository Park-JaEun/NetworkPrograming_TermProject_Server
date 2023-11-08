#include "pch.h"
#include "CBullet.h"

#include "CTimer.h"
#include "CCollider.h"

CBullet::CBullet() : m_fDir(1.f), m_fSpeed(0.f), m_bIsDown(false), m_fDegree(0.f)
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

	// 총알이 게임월드 밖으로 나가거나, 최대 사거리(MAX_BULLET_DISTANCE)를 넘어가면 삭제
	// 최대 사거리 계산
	float fDistance = sqrt(pow(GetFirstPos().x - vPos.x, 2) + pow(GetFirstPos().y - vPos.y, 2));
	if (!IsInWorld(vPos) || fDistance > MAX_BULLET_DISTANCE)
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