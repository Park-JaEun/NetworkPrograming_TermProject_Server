#include "pch.h"
#include "CBoss.h"
#include "CBullet.h"
#include "CTimer.h"
#include "CCollider.h"
#include "CObjectMgr.h"
#include <random>

int bossBulletId = 0;

int GetRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> uid(min, max);

	return uid(gen);
}

CBoss::CBoss() : m_iHP(10), m_vFirstPos{}, m_fSpeed(50.f), m_fMaxDistance(15.f),
m_bIsAppear(false), m_bHaveToAppear(false), m_fAttackTime(0.f),
m_eState(BOSS_STATE::IDLE), m_fDegree(0.f), m_fDieTime(0.f)
{
}

CBoss::~CBoss()
{
}

void CBoss::update()
{
	Vec2 vCurPos = GetPos();

	// HP가 0이하가 되면 사망
	if (m_iHP <= 0) {
		if (m_eState != BOSS_STATE::DIE) {
			m_eState = BOSS_STATE::DIE;
		}

		// 3초후 삭제
		if (m_fDieTime >= 3.f) {
			DeleteObject(this);
		}
		else
			m_fDieTime += DT;

		// 아래로 추락
		vCurPos.y += DT * 50.f * 3;
	}
	else {
		// 보스 등장
		if (m_bHaveToAppear && !m_bIsAppear) {
			vCurPos.y -= DT * m_fSpeed * 3;

			if (vCurPos.y <= 0.f) {
				m_bIsAppear = true;
				SetFirstPos(vCurPos);
			}
		}

		// 보스 등장후 위아래로 움직이는
		if (m_bIsAppear) {
			// 진행 방향으로 이동
			vCurPos.y += DT * m_fSpeed * 1;

			// 배회 거리 기준량을 넘었는지 확인
			float fDiff = abs(m_vFirstPos.y - vCurPos.y) - m_fMaxDistance;

			if (fDiff > 0.f) {
				// 방향 변경
				m_fSpeed *= -1.f;

				// 넘어선 만큼 반대방향으로 이동
				vCurPos.y += DT * m_fSpeed * 1;
			}

			// 1초마다 한 번씩 공격
			if (m_fAttackTime >= 1.f) {
				CreateFanBullet();
				m_fAttackTime = 0.f;	// 공격 후 공격 시간 초기화
				m_eState = BOSS_STATE::ATTACK;
			}
			else {
				if(m_eState != BOSS_STATE::IDLE)
					m_eState = BOSS_STATE::IDLE;
				m_fAttackTime += DT;	// 공격 시간 증가
			}
		}
	}

	SetPos(vCurPos);
}

void CBoss::CreateFanBullet()
{
	Vec2 vBulletPos = GetPos();
	// 각도 증가량
	float fDegreeInc;

	if(GetRandomNumber(0, 1))
		fDegreeInc = 15.f;
	else
		fDegreeInc = 20.f;

	for (int i = 0; i < 5; ++i) {
		CBullet* pBullet = new CBullet;

		pBullet->SetName(L"Boss Bullet");
		pBullet->SetSpeed(100.f);
		pBullet->SetPos(vBulletPos);
		pBullet->SetFirstPos(vBulletPos);
		pBullet->SetDir(DIR_LEFT);
		pBullet->SetDegree(150.f + (fDegreeInc * i));
		pBullet->SetID(bossBulletId++);
		pBullet->SetGroupType(GROUP_TYPE::BULLET_BOSS);
		pBullet->SetPlayerID(4);
		pBullet->CreateCollider();
		pBullet->GetCollider()->SetScale(Vec2(8.f, 8.f));

		CreateObject(pBullet, GROUP_TYPE::BULLET_BOSS);
	}
}

void CBoss::OnCollision(CCollider* _pOther)
{
}

void CBoss::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (dynamic_cast<CBullet*>(pOtherObj)) {
		if (((CBullet*)pOtherObj)->GetGroupType() == GROUP_TYPE::BULLET_PLAYER) {
			// 플레이어 총알과 충돌시 HP 감소
			if (m_iHP > 0)
				m_iHP -= 1;

			// 죽었으면 본인을 죽인 총알의 PlayerID를 받아옴.
			if (m_iHP == 0 && m_eState != BOSS_STATE::DIE) {
				int killPlayerId = ((CBullet*)pOtherObj)->GetPlayerID();

				CObject* pKillPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(killPlayerId));
				((CPlayer*)pKillPlayer)->PlusKillCount();
			}
		}
	}
}

void CBoss::ExitCollision(CCollider* _pOther)
{
}


