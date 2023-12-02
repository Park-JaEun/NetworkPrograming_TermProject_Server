#include "pch.h"
#include "CBoss.h"
#include "CBullet.h"
#include "CTimer.h"
#include "CCollider.h"
#include "CObjectMgr.h"
#include <random>

int bossBulletId = 0;

// ���� ���� ����
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

	// HP�� 0���ϰ� �Ǹ� ���
	if (m_iHP <= 0) {
		if (m_eState != BOSS_STATE::DIE) {
			m_eState = BOSS_STATE::DIE;
		}

		// 3���� ����
		if (m_fDieTime >= 3.f)
			DeleteObject(this);
		else
			m_fDieTime += DT;

		// �Ʒ��� �߶�
		vCurPos.y += DT * 50.f * 3;
	}
	else {
		// ���� ����
		if (m_bHaveToAppear && !m_bIsAppear) {
			vCurPos.y -= DT * m_fSpeed * 3;

			if (vCurPos.y <= 0.f) {
				m_bIsAppear = true;
				SetFirstPos(vCurPos);
			}
		}

		// ���� ������ ���Ʒ��� �����̴�
		if (m_bIsAppear) {
			// ���� �������� �̵�
			vCurPos.y += DT * m_fSpeed * 1;

			// ��ȸ �Ÿ� ���ط��� �Ѿ����� Ȯ��
			float fDiff = abs(m_vFirstPos.y - vCurPos.y) - m_fMaxDistance;

			if (fDiff > 0.f) {
				// ���� ����
				m_fSpeed *= -1.f;

				// �Ѿ ��ŭ �ݴ�������� �̵�
				vCurPos.y += DT * m_fSpeed * 1;
			}

			// 1�ʸ��� �� ���� 2���� ���� �� �������� ����
			if (m_fAttackTime >= 1.f) {
				CreateFanBullet();
				m_fAttackTime = 0.f;	// ���� �� ���� �ð� �ʱ�ȭ
			}
			else
				m_fAttackTime += DT;	// ���� �ð� ����
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

		// �Ѿ�	3�� �� 1���� �������� ����
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
	// ���� �̻��� �߻� �Լ�
	CObject* pTarget = CObjectMgr::GetInst()->FindObject(L"Player");
	Vec2 vMissilePos;

	CBullet* pMissile = new CBullet;
	pMissile->SetName(L"Boss Missile");
	pMissile->CreateCollider();

	// 1/2 Ȯ���� ���� �Ǵ� �Ʒ��� �̻��� ����
	int iRand = rand() % 2;

	if (iRand == 0) {
		// ������ ��, y��ǥ�� �÷��̾��� y��ǥ
		// x��ǥ�� �ڱ� �ڽ��� x��ǥ
		vMissilePos.x = GetPos().x;
		vMissilePos.y = pTarget->GetPos().y;

		pMissile->SetSpeed(700.f);
		pMissile->SetDir(DIR_LEFT);
		pMissile->GetCollider()->SetScale(Vec2(94.f, 27.f));
	}
	else {
		// �Ʒ��� ��, x��ǥ�� �÷��̾��� x��ǥ
		// y��ǥ�� ����
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
		// �÷��̾� �Ѿ˰� �浹�� HP ����
		if (m_iHP > 0)
			m_iHP -= 1;
	}
}

void CBoss::ExitCollision(CCollider* _pOther)
{
}


