#include "pch.h"
#include "CBullet.h"

#include "CTimer.h"
#include "CCollider.h"

#include "CTexture.h"

#include "CAnimator.h"
#include "CResourceMgr.h"

CBullet::CBullet() : m_fDir(1.f), m_fSpeed(200.f), m_bIsDown(false), m_fDegree(0.f)
{
}

CBullet::~CBullet()
{
}

void CBullet::update()
{
	Vec2 vPos = GetPos();

	if (m_fDegree != 0.f) {
		vPos.x += DT * m_fSpeed * cosf(m_fDegree);
		vPos.y += DT * m_fSpeed * sinf(m_fDegree);
	}
	else {
		if (m_bIsDown)
			vPos.y += DT * m_fSpeed * m_fDir;
		else
			vPos.x += DT * m_fSpeed * m_fDir;
	}
	
	SetPos(vPos);
	GetAnimator()->update();

	// 카메라나 게임월드 밖으로 나가면 삭제 이벤트 발생
	if (!IsInWorld(vPos) || !IsInCamera(vPos))
		DeleteObject(this);
}

void CBullet::render(HDC _dc)
{
	componentRender(_dc);
}

void CBullet::CreateAnimator(GROUP_TYPE _eType)
{
	switch (_eType) 
	{
	case GROUP_TYPE::BULLET_PLAYER: {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Haerin", L"texture\\characters\\haerin\\hearin.bmp");

		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"Player_Bullet",		// Animation Name
			pTexture,			// Texture Pointer
			Vec2(0.f, 217.f),				// Texture Left Top
			Vec2(14.f, 13.f),			// Frame Size
			Vec2(14.f, 0.f),			// Step
			0.07f,						// Duration
			Vec2(0.f, 0.f),				// Offset
			Vec2(2.f, 2.f),				// Scale
			4);							// Frame Count

		pAnimator->Play(L"Player_Bullet", true);	// 현재 애니메이션 지정

		SetAnimator(pAnimator);
	}
		break;

	case GROUP_TYPE::BULLET_MONSTER: {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Bunny", L"texture\\characters\\bunny\\bunny.bmp");

		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"Monster_Bullet",		// Animation Name
			pTexture,			// Texture Pointer
			Vec2(0.f, 288.f),				// Texture Left Top
			Vec2(18.f, 18.f),			// Frame Size
			Vec2(18.f, 0.f),			// Step
			0.07f,						// Duration
			Vec2(0.f, 0.f),				// Offset
			Vec2(1.f, 1.f),				// Scale
			4);							// Frame Count

		pAnimator->Play(L"Monster_Bullet", true);	// 현재 애니메이션 지정

		SetAnimator(pAnimator);
	}
		break;

	case GROUP_TYPE::BULLET_BOSS: {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Boss", L"texture\\characters\\boss\\boss.bmp");

		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"Boss_Bullet",				// Animation Name
			pTexture,					// Texture Pointer
			Vec2(0.f, 343.f),			// Texture Left Top
			Vec2(18.f, 18.f),			// Frame Size
			Vec2(18.f, 0.f),			// Step
			0.07f,						// Duration
			Vec2(0.f, 0.f),				// Offset
			Vec2(1.f, 1.f),				// Scale
			4);							// Frame Count

		pAnimator->Play(L"Boss_Bullet", true);	// 현재 애니메이션 지정

		SetAnimator(pAnimator);
		}
		break;

	case GROUP_TYPE::MISSILE_BOSS: {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Boss", L"texture\\characters\\boss\\boss.bmp");

		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		// 방향에 따라서 다른 애니메이션 재생
		if (!m_bIsDown) {
			pAnimator->CreateAnimation(
				L"Boss_Missile",		// Animation Name
				pTexture,				// Texture Pointer
				Vec2(0.f, 286.f),		// Texture Left Top
				Vec2(94.f, 57.f),		// Frame Size
				Vec2(0.f, 0.f),			// Step
				0.f,					// Duration
				Vec2(0.f, 0.f),			// Offset
				1);						// Frame Count
		}

		if (m_bIsDown) {
			pAnimator->CreateAnimation(
				L"Boss_Missile",		// Animation Name
				pTexture,				// Texture Pointer
				Vec2(0.f, 192.f),		// Texture Left Top
				Vec2(60.f, 94.f),		// Frame Size
				Vec2(0.f, 0.f),			// Step
				0.f,					// Duration
				Vec2(0.f, 0.f),			// Offset
				1);						// Frame Count
		}

		pAnimator->Play(L"Boss_Missile", true);	// 현재 애니메이션 지정

		SetAnimator(pAnimator);
		}
		break;
	}

	
}

void CBullet::SetDir(int _iDir)
{
	if (_iDir == DIR_RIGHT)
		m_fDir = 1.f;
	else if (_iDir == DIR_LEFT)
		m_fDir = -1.f;
	else if (_iDir == DIR_DOWN) {
		m_fDir = 1.f;
		m_bIsDown = true;
	}
}

void CBullet::OnCollision(CCollider* _pOther)
{
}

void CBullet::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster") {
		DeleteObject(this);
	}

	if (pOtherObj->GetName() == L"Boss") {
		DeleteObject(this);
	}

	if (pOtherObj->GetName() == L"Player") {
		DeleteObject(this);
	}
}

void CBullet::ExitCollision(CCollider* _pOther)
{
}