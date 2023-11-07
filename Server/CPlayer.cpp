#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"

#include "CKeyMgr.h"
#include "CTimer.h"

#include "CCollider.h"

CPlayer::CPlayer() : m_bDir(DIR_RIGHT), m_eState(PLAYER_STATE::IDLE), m_fSpeed(300.f), m_iHP(5), m_fDieTime(0.f)
{
}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	Vec2 vPos = GetPos();
	Vec2 vDummyPos{};
	bool bIsBoss = pMainScene->GetIsBoss();

	if (m_iHP <= 0) {
		if (m_bDir == DIR_RIGHT)
		else if (m_bDir == DIR_LEFT)

		// 1.5초뒤 아래로 추락
		if (m_fDieTime >= 1.5f)
			vPos.y += DT * 50.f * 3;
		else
			m_fDieTime += DT;
	}
	else {
		// 상
		if (KEY_HOLD(KEY::UP)) {
			vDummyPos = Vec2(vPos.x, vPos.y - m_fSpeed * DT);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.y -= m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.y -= m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
		}
		if (KEY_AWAY(KEY::UP)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 하
		if (KEY_HOLD(KEY::DOWN)) {
			vDummyPos = Vec2(vPos.x, vPos.y + m_fSpeed * DT);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.y += m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.y += m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
		}
		if (KEY_AWAY(KEY::DOWN)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 좌
		if (KEY_HOLD(KEY::LEFT)) {
			vDummyPos = Vec2(vPos.x - m_fSpeed * DT, vPos.y);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.x -= m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.x -= m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
			if (m_bDir != DIR_LEFT)
				m_bDir = DIR_LEFT;
		}
		if (KEY_AWAY(KEY::LEFT)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 우
		if (KEY_HOLD(KEY::RIGHT)) {
			vDummyPos = Vec2(vPos.x + m_fSpeed * DT, vPos.y);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.x += m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.x += m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
			if (m_bDir != DIR_RIGHT)
				m_bDir = DIR_RIGHT;
		}
		if (KEY_AWAY(KEY::RIGHT)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		if (KEY_TAP(KEY::SPACE)) {
			// 총알 발사
			CreateBullet();

			// 슈팅 이펙트 재생
		}

		// 플레이어 상태와 방향에 따라서 플레이되는 애니메이션 변경
		if (m_bDir == DIR_RIGHT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				break;
			case PLAYER_STATE::RUN:
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			}
		}
		else if (m_bDir == DIR_LEFT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				break;
			case PLAYER_STATE::RUN:
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			}
		}

	}

	SetPos(vPos);				// 위치 업데이트
}

void CPlayer::CreateBullet()
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Player Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetScale(Vec2(14.f, 13.f));

	if (m_bDir == DIR_RIGHT)
		pBullet->SetDir(DIR_RIGHT);
	else
		pBullet->SetDir(DIR_LEFT);

	pBullet->CreateCollider();
	pBullet->CreateAnimator(GROUP_TYPE::BULLET_PLAYER);

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
		--m_iHP;
	}

	if (pOtherObj->GetName() == L"Boss Bullet" || pOtherObj->GetName() == L"Boss Missile") {
		--m_iHP;
	}
}

void CPlayer::ExitCollision(CCollider* _pOther)
{
}
