#include "pch.h"
#include "CBoss.h"
#include "CBullet.h"
#include "CTimer.h"
#include "CCollider.h"
#include "CObjectMgr.h"
#include <random>

int bossBulletId = 0;

// 완전 랜덤 난수
int GetRandomNumber()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> uid(0, 1);

	return uid(gen);
}

CBoss::CBoss() : m_iHP(50), m_vFirstPos{}, m_fSpeed(50.f), m_fMaxDistance(15.f),
m_bIsAppear(false), m_bHaveToAppear(false), m_fAttackTime(0.f),
m_eState(BOSS_STATE::IDLE), m_fDegree(0.f),
m_fDieTime(0.f)
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
		if (m_fDieTime >= 3.f)
			DeleteObject(this);
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

			// 1초마다 한 번씩 2개의 패턴 중 랜덤으로 공격
			if (m_fAttackTime >= 1.f) {
				CreateFanBullet();
				m_fAttackTime = 0.f;	// 공격 후 공격 시간 초기화
			}
			else
				m_fAttackTime += DT;	// 공격 시간 증가
		}
	}

	SetPos(vCurPos);
}

void CBoss::CreateFanBullet()
{
	Vec2 vBulletPos = GetPos();
	int iCount = 0;

	for (int i = 0; i < 5; ++i) {
		int iRand = GetRandomNumber();

		// 총알	3개 중 1개는 생성하지 않음
		if (iRand == 0 && iCount < 3) {
			++iCount;
			continue;
		}

		CBullet* pBullet = new CBullet;

		pBullet->SetName(L"Boss Bullet");
		pBullet->SetSpeed(100.f);
		pBullet->SetPos(vBulletPos);
		pBullet->SetFirstPos(vBulletPos);
		pBullet->SetDir(DIR_LEFT);
		pBullet->SetDegree(59.0f + i / 10.f);
		pBullet->SetID(bossBulletId++);
		pBullet->SetGroupType(GROUP_TYPE::BULLET_BOSS);
		pBullet->SetPlayerID(4);
		pBullet->CreateCollider();
		pBullet->GetCollider()->SetScale(Vec2(8.f, 8.f));

		CreateObject(pBullet, GROUP_TYPE::BULLET_BOSS);
	}
}

void CBoss::CreateMissile()
{
	// 보스 미사일 발사 함수
	CObject* pTarget = CObjectMgr::GetInst()->FindObject(L"Player");
	Vec2 vMissilePos;

	CBullet* pMissile = new CBullet;
	pMissile->SetName(L"Boss Missile");
	pMissile->CreateCollider();

	// 1/2 확률로 왼쪽 또는 아래쪽 미사일 생성
	int iRand = rand() % 2;

	if (iRand == 0) {
		// 왼쪽일 때, y좌표는 플레이어의 y좌표
		// x좌표는 자기 자신의 x좌표
		vMissilePos.x = GetPos().x;
		vMissilePos.y = pTarget->GetPos().y;

		pMissile->SetSpeed(700.f);
		pMissile->SetDir(DIR_LEFT);
		pMissile->GetCollider()->SetScale(Vec2(94.f, 27.f));
	}
	else {
		// 아래일 때, x좌표는 플레이어의 x좌표
		// y좌표는 맨위
		vMissilePos.x = pTarget->GetPos().x;
		vMissilePos.y = -150.f;

		pMissile->SetSpeed(500.f);
		pMissile->SetDir(DIR_DOWN);
		pMissile->GetCollider()->SetScale(Vec2(30.f, 94.f));
	}

	pMissile->SetPos(vMissilePos);
	pMissile->SetFirstPos(vMissilePos);

	CreateObject(pMissile, GROUP_TYPE::MISSILE_BOSS);
}

void CBoss::OnCollision(CCollider* _pOther)
{
}

void CBoss::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player Bullet") {
		// 플레이어 총알과 충돌시 HP 감소
		if (m_iHP > 0)
			m_iHP -= 1;
	}
}

void CBoss::ExitCollision(CCollider* _pOther)
{
}


