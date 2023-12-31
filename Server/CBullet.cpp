#include "pch.h"
#include "CBullet.h"

#include "CTimer.h"
#include "CCollider.h"

CBullet::CBullet() : m_fDir(1.f), m_fSpeed(0.f), m_bIsDown(false), m_fDegree(0.f), m_vFirstPos(Vec2(0.f, 0.f)), m_iID(0)
{
}

CBullet::~CBullet()
{
}

void CBullet::update()
{
	Vec2 vPos = GetPos();

	if (m_fDegree != 0.f) {
		float fRadian = m_fDegree * 3.141592f / 180.f;

		vPos.x += DT * m_fSpeed * cosf(fRadian);
		vPos.y += DT * m_fSpeed * sinf(fRadian);
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
	double fDistance = sqrt(pow(GetFirstPos().x - vPos.x, 2) + pow(GetFirstPos().y - vPos.y, 2));
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

	// 몬스터와 충돌하면 총알 삭제
	if (dynamic_cast<CMonster*>(pOtherObj)) {
		if (((CMonster*)pOtherObj)->GetGroupType() == GROUP_TYPE::MONSTER) {
			DeleteObject(this);
		}
	}

	if (pOtherObj->GetName() == L"Boss") {
		DeleteObject(this);
	}

	if (pOtherObj->GetName() == L"Player0") {
		DeleteObject(this);
	}
	else if (pOtherObj->GetName() == L"Player1") {
		DeleteObject(this);
	}
	else if (pOtherObj->GetName() == L"Player2") {
		DeleteObject(this);
	}
}

void CBullet::ExitCollision(CCollider* _pOther)
{
}