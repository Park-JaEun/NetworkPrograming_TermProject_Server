#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CKeyMgr.h"
#include "CTimer.h"
#include "CCollider.h"
#include "CObjectMgr.h"
#include "CBoss.h"

CPlayer::CPlayer() : m_bDir(DIR_RIGHT), m_eState(PLAYER_STATE::IDLE),
					 m_fSpeed(300.f), m_iHP(3), m_iLife(3),
					 m_fDieTime(0.f), m_fResurrectTime(0.f), m_bIsGameOver(false),
					 m_iKillCount(0), m_iBunnyCount(0), m_iCookieCount(0)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::update()
{
}

void CPlayer::CreateBullet(int id, int bulletId)
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Player" + std::to_wstring(id) + L" Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetFirstPos(vBulletPos);
	pBullet->SetID(bulletId);
	pBullet->SetPlayerID(id);
	pBullet->SetGroupType(GROUP_TYPE::BULLET_PLAYER);

	if (m_bDir == DIR_RIGHT)
		pBullet->SetDir(DIR_RIGHT);
	else
		pBullet->SetDir(DIR_LEFT);

	pBullet->CreateCollider();

	pBullet->SetSpeed(700.f);
	pBullet->GetCollider()->SetScale(Vec2(25.f, 25.f));

	CreateObject(pBullet, GROUP_TYPE::BULLET_PLAYER);
}


void CPlayer::OnCollision(CCollider* _pOther)
{
}

void CPlayer::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster Bullet") {
		/*if (m_iHP > 0)
			--m_iHP;*/
	}

	if (pOtherObj->GetName() == L"Boss Bullet") {
		/*if (m_iHP > 0)
			--m_iHP;*/
	}
}

void CPlayer::ExitCollision(CCollider* _pOther)
{
}
