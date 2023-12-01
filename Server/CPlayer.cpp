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
	// 게임 오버시 return
	if (m_bIsGameOver)
		return;

	//Vec2 vPos = GetPos();
	//Vec2 vDummyPos{};
	//
	//CBoss* pBoss = (CBoss*)CObjectMgr::GetInst()->FindObject(L"Boss");
	//bool bIsBoss = pBoss->GetHaveToAppear();


	//if (m_iHP <= 0) {
	//	if (m_eState != PLAYER_STATE::DIE) {
	//		m_eState = PLAYER_STATE::DIE;
	//	}

	//	// 1.5초뒤 아래로 추락
	//	if (m_fDieTime >= 1.5f && m_fDieTime <= 4.5)
	//		vPos.y += DT * 50.f * 3;
	//	else
	//		m_fDieTime += DT;

	//	// 3초뒤 부활
	//	if (!m_bIsGameOver && m_iLife >= 1 && m_fResurrectTime >= 3.0f && m_fDieTime >= 1.5f) {
	//		m_iLife -= 1;
	//		if (m_iLife == 0) {
	//			m_bIsGameOver = true;
	//			return;
	//		}
	//		m_iHP = 3;
	//		m_fDieTime = 0.f;
	//		m_fResurrectTime = 0.f;
	//		m_eState = PLAYER_STATE::IDLE;
	//		vPos.y = 0.f;
	//	}
	//	else if (m_fDieTime >= 1.5f) {
	//		m_fResurrectTime += DT;
	//	}
	//}
	//else {
	//	// 상
	//	if (KEY_HOLD(KEY::UP)) {
	//		vDummyPos = Vec2(vPos.x, vPos.y - m_fSpeed * DT);

	//		if (IsInWorld(vDummyPos) && !bIsBoss)
	//			vPos.y -= m_fSpeed * DT;
	//		else if (IsInBossRoom(vDummyPos) && bIsBoss)
	//			vPos.y -= m_fSpeed * DT;

	//		m_eState = PLAYER_STATE::RUN;
	//	}
	//	if (KEY_AWAY(KEY::UP)) {
	//		m_eState = PLAYER_STATE::IDLE;
	//	}

	//	// 하
	//	if (KEY_HOLD(KEY::DOWN)) {
	//		vDummyPos = Vec2(vPos.x, vPos.y + m_fSpeed * DT);

	//		if (IsInWorld(vDummyPos) && !bIsBoss)
	//			vPos.y += m_fSpeed * DT;
	//		else if (IsInBossRoom(vDummyPos) && bIsBoss)
	//			vPos.y += m_fSpeed * DT;

	//		m_eState = PLAYER_STATE::RUN;
	//	}
	//	if (KEY_AWAY(KEY::DOWN)) {
	//		m_eState = PLAYER_STATE::IDLE;
	//	}

	//	// 좌
	//	if (KEY_HOLD(KEY::LEFT)) {
	//		vDummyPos = Vec2(vPos.x - m_fSpeed * DT, vPos.y);

	//		if (IsInWorld(vDummyPos) && !bIsBoss)
	//			vPos.x -= m_fSpeed * DT;
	//		else if (IsInBossRoom(vDummyPos) && bIsBoss)
	//			vPos.x -= m_fSpeed * DT;

	//		m_eState = PLAYER_STATE::RUN;
	//		if (m_bDir != DIR_LEFT)
	//			m_bDir = DIR_LEFT;
	//	}
	//	if (KEY_AWAY(KEY::LEFT)) {
	//		m_eState = PLAYER_STATE::IDLE;
	//	}

	//	// 우
	//	if (KEY_HOLD(KEY::RIGHT)) {
	//		vDummyPos = Vec2(vPos.x + m_fSpeed * DT, vPos.y);

	//		if (IsInWorld(vDummyPos) && !bIsBoss)
	//			vPos.x += m_fSpeed * DT;
	//		else if (IsInBossRoom(vDummyPos) && bIsBoss)
	//			vPos.x += m_fSpeed * DT;

	//		m_eState = PLAYER_STATE::RUN;
	//		if (m_bDir != DIR_RIGHT)
	//			m_bDir = DIR_RIGHT;
	//	}
	//	if (KEY_AWAY(KEY::RIGHT)) {
	//		m_eState = PLAYER_STATE::IDLE;
	//	}

	//	if (KEY_TAP(KEY::SPACE)) {
	//		// 총알 발사
	//		CreateBullet();
	//	}
	//}

	//SetPos(vPos);				// 위치 업데이트
}

void CPlayer::CreateBullet(int id)
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Player" + std::to_wstring(id) + L" Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetFirstPos(vBulletPos);

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
		if (m_iHP > 0)
			--m_iHP;
	}

	if (pOtherObj->GetName() == L"Boss Bullet" || pOtherObj->GetName() == L"Boss Missile") {
		if (m_iHP > 0)
			--m_iHP;
	}
}

void CPlayer::ExitCollision(CCollider* _pOther)
{
}
