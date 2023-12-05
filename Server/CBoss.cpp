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

	// HP�� 0���ϰ� �Ǹ� ���
	if (m_iHP <= 0) {
		if (m_eState != BOSS_STATE::DIE) {
			m_eState = BOSS_STATE::DIE;
		}

		// 3���� ����
		if (m_fDieTime >= 3.f) {
			DeleteObject(this);
		}
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

			// 1�ʸ��� �� ���� ����
			if (m_fAttackTime >= 1.f) {
				CreateFanBullet();
				m_fAttackTime = 0.f;	// ���� �� ���� �ð� �ʱ�ȭ
				m_eState = BOSS_STATE::ATTACK;
			}
			else {
				if(m_eState != BOSS_STATE::IDLE)
					m_eState = BOSS_STATE::IDLE;
				m_fAttackTime += DT;	// ���� �ð� ����
			}
		}
	}

	SetPos(vCurPos);
}

void CBoss::CreateFanBullet()
{
	Vec2 vBulletPos = GetPos();
	// ���� ������
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
			// �÷��̾� �Ѿ˰� �浹�� HP ����
			if (m_iHP > 0)
				m_iHP -= 1;

			// �׾����� ������ ���� �Ѿ��� PlayerID�� �޾ƿ�.
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


