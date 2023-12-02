#include "pch.h"
#include "CMonster.h"
#include "CBullet.h"
#include "CObjectMgr.h"
#include "CTimer.h"
#include "CCollider.h"

int monsterBulletId = 0;

CMonster::CMonster() : m_fSpeed(50.f), m_vFirstPos{}, m_fMaxDistance(50.f), m_bDir(DIR_RIGHT), 
					   m_iHP(3), m_eState(MONSTER_STATE::MOVE), m_fAttackCoolTime(0.f), m_fDieTime(0.f)
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	Vec2 vCurPos = GetPos();

	// HP�� 0���ϰ� �Ǹ� 1�ʵ� ����
	if (m_iHP <= 0) {
		if (m_eState != MONSTER_STATE::DIE) {
			m_eState = MONSTER_STATE::DIE;
		}
		else {
			m_fDieTime += DT;

			if (m_fDieTime > 1.f)
				DeleteObject(this);
		}
	}
	// HP�� 0���� ���� ��
	else {
		// ���� �þ߿� �÷��̾ ������ ����
		for (int i = 0; i < MAX_PLAYER; ++i) {
			// ��� �÷��̾ �þ߿� ������ üũ
			if (IsInSight(vCurPos, 300.f, L"Player" + std::to_wstring(i))) {
				m_eState = MONSTER_STATE::ATTACK;
				break;
			}
			else
				m_eState = MONSTER_STATE::MOVE;
		}

		// �����ϰ� ���� �� 1�ʿ� �ѹ��� �Ѿ� �߻�
		if (m_eState == MONSTER_STATE::ATTACK) {
			m_fAttackCoolTime += DT;

			if (m_fAttackCoolTime > 0.6f) {
				CreateBullet(3, monsterBulletId++);
				m_fAttackCoolTime = 0.f;
			}
		}
		else {
			m_fAttackCoolTime = 0.f;
		}

		// �����̰� ���� ��
		if (m_eState == MONSTER_STATE::MOVE) {

			if (m_bDir == DIR_RIGHT) {
				// ���� �������� �̵�
				vCurPos.x += DT * m_fSpeed * 1;

				// ��ȸ �Ÿ� ���ط��� �Ѿ����� Ȯ��
				float fDiff = abs(m_vFirstPos.x - vCurPos.x) - m_fMaxDistance;

				if (fDiff > 0.f) {
					// ���� ����
					m_bDir = !m_bDir;
					// �Ѿ ��ŭ �ݴ�������� �̵�
					vCurPos.x += fDiff * -1;
				}
			}

			if (m_bDir == DIR_LEFT) {
				// ���� �������� �̵�
				vCurPos.x += DT * m_fSpeed * -1;

				// ��ȸ �Ÿ� ���ط��� �Ѿ����� Ȯ��
				float fDiff = abs(m_vFirstPos.x - vCurPos.x) - m_fMaxDistance;

				if (fDiff > 0.f) {
					// ���� ����
					m_bDir = !m_bDir;
					// �Ѿ ��ŭ �ݴ�������� �̵�
					vCurPos.x += fDiff * 1;
				}
			}
		}
	}

	SetPos(vCurPos);
}

void CMonster::CreateBullet(int id, int bulletId)
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Monster Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetFirstPos(vBulletPos);
	pBullet->SetID(bulletId);
	pBullet->SetPlayerID(id);
	pBullet->SetGroupType(GROUP_TYPE::BULLET_MONSTER);

	if (m_bDir == DIR_RIGHT)
		pBullet->SetDir(DIR_RIGHT);
	else
		pBullet->SetDir(DIR_LEFT);

	pBullet->CreateCollider();

	pBullet->SetSpeed(700.f);
	pBullet->GetCollider()->SetScale(Vec2(18.f, 18.f));

	CreateObject(pBullet, GROUP_TYPE::BULLET_MONSTER);
}

bool CMonster::IsInSight(Vec2 _vPos, float _fDistance, const std::wstring& _strName)
{
	// vPos�� �������� _fDistance �Ÿ� �ȿ� _strName �±׸� ���� ������Ʈ�� �ִ��� Ȯ��
	std::vector<CObject*> vecObject = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::PLAYER);

	for (auto& pObj : vecObject) {
		if (pObj->GetName() == _strName) {
			Vec2 vOtherPos = pObj->GetPos();

			float fDistance = sqrtf(powf(_vPos.x - vOtherPos.x, 2) + powf(_vPos.y - vOtherPos.y, 2));

			if (fDistance < _fDistance) {
				// ����� �����ʿ� ������ DIR_RIGHT, ���ʿ� ������ DIR_LEFT
				if (_vPos.x - vOtherPos.x > 0)
					m_bDir = DIR_LEFT;
				else
					m_bDir = DIR_RIGHT;

				return true;
			}
		}
	}
	return false;
}

void CMonster::OnCollision(CCollider* _pOther)
{
}

void CMonster::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (dynamic_cast<CBullet*>(pOtherObj)) {
		if (((CBullet*)pOtherObj)->GetGroupType() == GROUP_TYPE::BULLET_PLAYER) {
			// �÷��̾� �Ѿ˰� �浹�� HP ����
			if (m_iHP > 0)
				m_iHP -= 1;

			// �׾����� ������ ���� �Ѿ��� PlayerID�� �޾ƿ�.
			if (m_iHP == 0) {
				int killPlayerId = ((CBullet*)pOtherObj)->GetPlayerID();

				CObject* pKillPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(killPlayerId));
				((CPlayer*)pKillPlayer)->PlusKillCount();
			}
		}
	}
}

void CMonster::ExitCollision(CCollider* _pOther)
{
}


