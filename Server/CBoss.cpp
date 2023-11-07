#include "pch.h"
#include "CBoss.h"
#include "CBullet.h"
#include "CTimer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCollider.h"

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

	// HP�� 0���ϰ� �Ǹ� �״� �ִϸ��̼� ����� ����
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
		switch (m_eState) {
		case BOSS_STATE::IDLE:
			break;
		case BOSS_STATE::ATTACK:
			break;
		}

		// ���� ���� �ִϸ��̼�
		if (m_bHaveToAppear && !m_bIsAppear) {
			vCurPos.y -= DT * m_fSpeed * 3;

			if (vCurPos.y <= 0.f) {
				m_bIsAppear = true;
				SetFirstPos(vCurPos);
			}
		}

		// ���� ������ ���Ʒ��� �����̴� �ִϸ��̼�
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
				int iRand = rand() % 2;

				switch (iRand) {
				case 0:
					CreateMissile();
					break;
				case 1:
					CreateMissile();
					CreateFanBullet();
					break;
				}

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

	for (int i = 0; i < 15; ++i) {
		int iRand = rand() % 2;

		if (iRand == 0 && iCount < 3) {
			++iCount;
			continue;
		}

		CBullet* pBullet = new CBullet;

		pBullet->SetName(L"Boss Bullet");
		pBullet->SetSpeed(200.f);
		pBullet->SetPos(vBulletPos);
		pBullet->SetDir(DIR_LEFT);
		pBullet->SetDegree(59.0f + i / 10.f);
		pBullet->CreateCollider();
		pBullet->CreateAnimator(GROUP_TYPE::BULLET_BOSS);
		pBullet->GetCollider()->SetScale(Vec2(8.f, 8.f));

		CreateObject(pBullet, GROUP_TYPE::BULLET_BOSS);
	}
}

void CBoss::CreateMissile()
{
	// ���� �̻��� �߻� �Լ�
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CObject* pTarget = pScene->FindObject(L"Player");
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
	pMissile->CreateAnimator(GROUP_TYPE::MISSILE_BOSS);

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


