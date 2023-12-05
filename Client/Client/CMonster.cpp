#include "pch.h"
#include "CMonster.h"
#include "CBullet.h"

#include "CTimer.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CResourceMgr.h"
#include "CPathMgr.h"

#include "CTexture.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CMonster::CMonster() : m_fSpeed(50.f), m_vFirstPos{}, m_fMaxDistance(50.f), m_bDir(DIR_RIGHT), 
					   m_iHP(3), m_eState(MONSTER_STATE::MOVE), m_fDieTime(0.f), m_vPrevPos(Vec2(0.f, 0.f))
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	PredictMonsterPos();	// ����
	InterpolatePos();	// ����

	if (m_bDir == DIR_RIGHT) {
		switch (m_eState)
		{
		case MONSTER_STATE::IDLE:
			GetAnimator()->Play(L"Bunny_Idle_Right", true);
			break;
		case MONSTER_STATE::MOVE:
			GetAnimator()->Play(L"Bunny_Move_Right", true);
			break;
		case MONSTER_STATE::HIT:
			break;
		case MONSTER_STATE::ATTACK: 
		{
			GetAnimator()->Play(L"Bunny_Attack_Right", false);
			if(GetAnimator()->FindAnimation(L"Bunny_Attack_Right")->IsFinish())
				GetAnimator()->FindAnimation(L"Bunny_Attack_Right")->SetFrame(0);
		}
			break;
		case MONSTER_STATE::DIE:
			GetAnimator()->Play(L"Bunny_Die_Right", false);
			break;
		default:
			break;
		}
	}
	else if (m_bDir == DIR_LEFT) {
		switch (m_eState)
		{
		case MONSTER_STATE::IDLE:
			GetAnimator()->Play(L"Bunny_Idle_Left", true);
			break;
		case MONSTER_STATE::MOVE:
			GetAnimator()->Play(L"Bunny_Move_Left", true);
			break;
		case MONSTER_STATE::HIT:
			break;
		case MONSTER_STATE::ATTACK: 
		{
			GetAnimator()->Play(L"Bunny_Attack_Left", false);
			if(GetAnimator()->FindAnimation(L"Bunny_Attack_Left")->IsFinish())
				GetAnimator()->FindAnimation(L"Bunny_Attack_Left")->SetFrame(0);
		}
			break;
		case MONSTER_STATE::DIE:
			GetAnimator()->Play(L"Bunny_Die_Left", false);
			break;
		default:
			break;
		}
	}

	GetAnimator()->update();	// �ִϸ����� ������Ʈ

	// �׾��� ��, ���� ��ġ�� ���� ��ġ�� ��ȭ�� ���� �ð��� 1.5�ʰ� ������ �������� �ʴ´ٰ� �Ǵ�
	if (m_eState == MONSTER_STATE::DIE) {
		if ((int)m_vPrevPos.x == (int)GetPos().x) {
			m_fDieTime += DT;
			if (m_fDieTime > 1.5f) {
				DeleteObject(this);
			}
		}
		else {
			m_fDieTime = 0.f;
			m_vPrevPos = GetPos();
		}
	}
	else {
		m_vPrevPos = GetPos();
	}
}

void CMonster::render(HDC _dc)
{
	componentRender(_dc);
}

void CMonster::CreateAnimator()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Bunny", L"texture\\characters\\bunny\\bunny.bmp");

	// Animator �����
	CAnimator* pAnimator = new CAnimator;

	// Animator�� Object ����
	pAnimator->SetObj(this);

	// Animation �����
	pAnimator->CreateAnimation(
		L"Bunny_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Idle_Left",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 144.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Move_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 48.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.3f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		4);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Move_Left",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 192.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.3f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		4);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Attack_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 96.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Attack_Left",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 240.f),			// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Die_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 306.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.05f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		20);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Die_Left",			// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 354.f),			// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.05f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		20);						// Frame Count

	pAnimator->Play(L"Bunny_Move_Right", true);	// ���� �ִϸ��̼� ����

	SetAnimator(pAnimator);
}

void CMonster::CreateBullet()
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Monster Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetFirstPos(vBulletPos);

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

void CMonster::PredictMonsterPos()
{
	// ������ ������ ����Ͽ� ������ ��ġ ����
	Vec2 vPos = GetPos();
	float fInterforate = 10.f;

	// �� x��ǥ���� ���� x��ǥ�� �� ũ�ٸ�
	if (vPos.x > m_vPrevPos.x)
	{
		vPos.x += (m_fSpeed / fInterforate) * DT;
	}
	else if (vPos.x < m_vPrevPos.x)
	{
		vPos.x -= (m_fSpeed / fInterforate) * DT;
	}

	SetPos(vPos);
}

void CMonster::InterpolatePos()
{
	// ���� ����
	// ���� �����ǰ� ���� ������ ���� ���̸� ����
	Vec2 vPos = GetPos();
	Vec2 vPrevPos = m_vPrevPos;
	Vec2 interpolatePos = vPos;

	// ���� ����
	// 0 ~ 1 ������ ��
	float fInterpolate = 0.5f;

	interpolatePos.x = LerpX(vPrevPos.x, vPos.x, fInterpolate);
	interpolatePos.y = LerpY(vPrevPos.y, vPos.y, fInterpolate);

	SetPos(interpolatePos);
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


