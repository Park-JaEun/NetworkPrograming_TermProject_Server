#include "pch.h"
#include "CBoss.h"
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

CBoss::CBoss() :  m_iHP(50), m_vFirstPos{}, m_fSpeed(50.f), m_fMaxDistance(15.f), 
				  m_bIsAppear(false), m_bHaveToAppear(false), m_fAttackTime(0.f), 
				  m_eState(BOSS_STATE::NOT_APPEAR), m_EffectAnimator(nullptr), m_fDegree(0.f),
				  m_fDieTime(0.f)
{
}

CBoss::~CBoss()
{
}

void CBoss::update()
{
	switch (m_eState)
	{
	case BOSS_STATE::IDLE:
		GetAnimator()->Play(L"Boss_Idle", true);
		break;

	case BOSS_STATE::ATTACK:
	{
		GetAnimator()->Play(L"Boss_Attack", true);
		if (m_EffectAnimator != nullptr) {
			m_EffectAnimator->Play(L"Attack_Effect", false);
			m_EffectAnimator->FindAnimation(L"Attack_Effect")->SetFrame(0);
		}
	}
	break;

	case BOSS_STATE::DIE:
		GetAnimator()->Play(L"Boss_Die", true);
		break;
	}

	if (m_EffectAnimator != nullptr)
		m_EffectAnimator->update();

	GetAnimator()->update();	// 애니메이터 업데이트
}

void CBoss::render(HDC _dc)
{
	if (m_EffectAnimator != nullptr)
		m_EffectAnimator->render(_dc);

	componentRender(_dc);
}

void CBoss::CreateAnimator()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Boss", L"texture\\characters\\boss\\boss.bmp");

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Boss_Idle",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 0.f),		// Texture Left Top
		Vec2(33.f, 64.f),	// Frame Size
		Vec2(0.f, 0.f),		// Step
		0.f,				// Duration
		Vec2(0.f, 0.f),		// Offset
		1);					// Frame Count

	pAnimator->CreateAnimation(
		L"Boss_Attack",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 64.f),		// Texture Left Top
		Vec2(33.f, 64.f),	// Frame Size
		Vec2(0.f, 0.f),		// Step
		0.f,				// Duration
		Vec2(0.f, 0.f),		// Offset
		1);					// Frame Count

	pAnimator->CreateAnimation(
		L"Boss_Die",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 128.f),		// Texture Left Top
		Vec2(33.f, 64.f),	// Frame Size
		Vec2(0.f, 0.f),		// Step
		0.f,				// Duration
		Vec2(0.f, 0.f),		// Offset
		1);					// Frame Count

	m_EffectAnimator->CreateAnimation(
		L"Attack_Effect",		// Animation Name
		pTexture,				// Texture Pointer
		Vec2(0.f, 361.f),		// Texture Left Top
		Vec2(67.f, 67.f),		// Frame Size
		Vec2(67.f, 0.f),		// Step
		0.1f,					// Duration
		Vec2(0.f, -50.f),		// Offset
		Vec2(4.f, 4.f),			// Scale
		11);					// Frame Count

	pAnimator->FindAnimation(L"Boss_Idle")->SetScale(Vec2(5.f, 5.f));
	pAnimator->FindAnimation(L"Boss_Attack")->SetScale(Vec2(5.f, 5.f));
	pAnimator->FindAnimation(L"Boss_Die")->SetScale(Vec2(5.f, 5.f));

	pAnimator->Play(L"Boss_Idle", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
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
		pBullet->SetFirstPos(vBulletPos);
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
	// 보스 미사일 발사 함수
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CObject* pTarget = pScene->FindObject(L"Player");
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
	pMissile->CreateAnimator(GROUP_TYPE::MISSILE_BOSS);

	CreateObject(pMissile, GROUP_TYPE::MISSILE_BOSS);
}

void CBoss::OnCollision(CCollider* _pOther)
{
}

void CBoss::EnterCollision(CCollider* _pOther)
{
}

void CBoss::ExitCollision(CCollider* _pOther)
{
}


