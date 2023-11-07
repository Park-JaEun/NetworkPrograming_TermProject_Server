#include "pch.h"
#include "CMonster.h"
#include "CBullet.h"

#include "CTimer.h"
#include "CCollider.h"

CMonster::CMonster() : m_fSpeed(50.f), m_vFirstPos{}, m_fMaxDistance(50.f), m_bDir(DIR_RIGHT), m_iHP(3), m_eState(MONSTER_STATE::MOVE)
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	Vec2 vCurPos = GetPos();

	// HP�� 0���ϰ� �Ǹ� �״� �ִϸ��̼� ����� ����
	if (m_iHP <= 0) {
		if (m_eState != MONSTER_STATE::DIE)
			m_eState = MONSTER_STATE::DIE;

		DeleteObject(this);
	}
	// HP�� 0���� ���� ��
	else {
		// ���� �þ߿� �÷��̾ ������ ����
		if (IsInSight(GetPos(), 300.f, L"Player")) {
			m_eState = MONSTER_STATE::ATTACK;

			if (m_bDir == DIR_RIGHT)
			if (m_bDir == DIR_LEFT)
		}
		// �÷��̾ ������ �ٽ� ������
		else {
			m_eState = MONSTER_STATE::MOVE;
		}

		// �����ϰ� ���� ��
		if (m_eState == MONSTER_STATE::ATTACK) {

			CreateBullet();
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

	if (m_bDir == DIR_RIGHT) {
		switch (m_eState)
		{
		case MONSTER_STATE::IDLE:
			break;
		case MONSTER_STATE::MOVE:
			break;
		case MONSTER_STATE::HIT:
			break;
		case MONSTER_STATE::ATTACK:
			break;
		case MONSTER_STATE::DIE:
			break;
		default:
			break;
		}
	}
	else if (m_bDir == DIR_LEFT) {
		switch (m_eState)
		{
		case MONSTER_STATE::IDLE:
			break;
		case MONSTER_STATE::MOVE:
			break;
		case MONSTER_STATE::HIT:
			break;
		case MONSTER_STATE::ATTACK:
			break;
		case MONSTER_STATE::DIE:
			break;
		default:
			break;
		}
	}

	SetPos(vCurPos);
}

void CMonster::CreateBullet()
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Monster Bullet");
	pBullet->SetPos(vBulletPos);

	if (m_bDir == DIR_RIGHT)
		pBullet->SetDir(DIR_RIGHT);
	else
		pBullet->SetDir(DIR_LEFT);

	pBullet->CreateCollider();
	pBullet->CreateAnimator(GROUP_TYPE::BULLET_MONSTER);

	pBullet->SetSpeed(700.f);
	pBullet->GetCollider()->SetScale(Vec2(18.f, 18.f));

	CreateObject(pBullet, GROUP_TYPE::BULLET_MONSTER);
}

bool CMonster::IsInSight(Vec2 _vPos, float _fDistance, const std::wstring& _strName)
{
	// vPos�� �������� _fDistance �Ÿ� �ȿ� _strName �±׸� ���� ������Ʈ�� �ִ��� Ȯ��
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (pCurScene == nullptr)
		return false;

	std::vector<CObject*> vecObject = pCurScene->GetGroupObject(GROUP_TYPE::PLAYER);

	for (auto& pObj : vecObject) {
		if (pObj->GetName() == _strName) {
			Vec2 vOtherPos = pObj->GetPos();

			float fDistance = sqrtf(powf(_vPos.x - vOtherPos.x, 2) + powf(_vPos.y - vOtherPos.y, 2));

			if (fDistance < _fDistance) {
				// ����� ������ ������ DIR_RIGHT, ���ʿ� ������ DIR_LEFT
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

	if (pOtherObj->GetName() == L"Player Bullet") {
		// �÷��̾� �Ѿ˰� �浹�� HP ����
		if (m_iHP > 0)
			m_iHP -= 1;
	}
}

void CMonster::ExitCollision(CCollider* _pOther)
{
}


