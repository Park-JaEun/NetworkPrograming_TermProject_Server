#include "pch.h"
#include "CBullet.h"
#include "CPlayer.h"
#include "CTimer.h"
#include "CCollider.h"

#include "CTexture.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CAnimator.h"
#include "CResourceMgr.h"

CBullet::CBullet() : m_fDir(1.f), m_fSpeed(200.f), m_bIsDown(false), m_fDegree(0.f), m_vFirstPos(Vec2(0.f, 0.f))
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

	// 총알이 게임월드 밖으로 나가거나, 최대 사거리(MAX_BULLET_DISTANCE)를 넘어가면 삭제
	// 최대 사거리 계산
	double fDistance = sqrt(pow(GetFirstPos().x - vPos.x, 2) + pow(GetFirstPos().y - vPos.y, 2));
	if (!IsInWorld(vPos) || fDistance > MAX_BULLET_DISTANCE)
		DeleteObject(this);
}

void CBullet::render(HDC _dc)
{
	componentRender(_dc);
}

void CBullet::CreateMinjiBullet()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Minji", L"texture\\characters\\minji\\minji.bmp");

	CAnimator* pAnimator = new CAnimator;

	pAnimator->SetObj(this);

	pAnimator->CreateAnimation(
		L"Minji_Bullet",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 229.f),				// Texture Left Top
		Vec2(14.f, 13.f),			// Frame Size
		Vec2(14.f, 0.f),			// Step
		0.07f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		Vec2(2.f, 2.f),				// Scalez
		4);							// Frame Count

	pAnimator->Play(L"Minji_Bullet", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CBullet::CreateDanielleBullet()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Danielle", L"texture\\characters\\danielle\\danielle.bmp");

	CAnimator* pAnimator = new CAnimator;

	pAnimator->SetObj(this);

	pAnimator->CreateAnimation(
		L"Danielle_Bullet",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 229.f),				// Texture Left Top
		Vec2(14.f, 13.f),			// Frame Size
		Vec2(14.f, 0.f),			// Step
		0.07f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		Vec2(2.f, 2.f),				// Scale
		4);							// Frame Count

	pAnimator->Play(L"Danielle_Bullet", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CBullet::CreateHaerinBullet()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Haerin", L"texture\\characters\\haerin\\hearin.bmp");

	CAnimator* pAnimator = new CAnimator;

	pAnimator->SetObj(this);

	pAnimator->CreateAnimation(
		L"Haerin_Bullet",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 217.f),				// Texture Left Top
		Vec2(14.f, 13.f),			// Frame Size
		Vec2(14.f, 0.f),			// Step
		0.07f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		Vec2(2.f, 2.f),				// Scale
		4);							// Frame Count

	pAnimator->Play(L"Haerin_Bullet", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CBullet::CreateHannieBullet()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hannie", L"texture\\characters\\hannie\\hannie.bmp");

	CAnimator* pAnimator = new CAnimator;

	pAnimator->SetObj(this);

	pAnimator->CreateAnimation(
		L"Hannie_Bullet",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 235.f),				// Texture Left Top
		Vec2(14.f, 13.f),			// Frame Size
		Vec2(14.f, 0.f),			// Step
		0.07f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		Vec2(2.f, 2.f),				// Scale
		4);							// Frame Count

	pAnimator->Play(L"Hannie_Bullet", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CBullet::CreateHyeinBullet()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hyein", L"texture\\characters\\hyein\\hyein.bmp");

	CAnimator* pAnimator = new CAnimator;

	pAnimator->SetObj(this);

	pAnimator->CreateAnimation(
		L"Hyein_Bullet",		// Animation Name
		pTexture,			// Texture Pointer
		Vec2(0.f, 223.f),				// Texture Left Top
		Vec2(14.f, 13.f),			// Frame Size
		Vec2(14.f, 0.f),			// Step
		0.07f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		Vec2(2.f, 2.f),				// Scale
		4);							// Frame Count

	pAnimator->Play(L"Hyein_Bullet", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CBullet::CreateAnimator(GROUP_TYPE _eType)
{
	switch (_eType) 
	{
	case GROUP_TYPE::BULLET_PLAYER: {
		CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Player");

		switch (pPlayer->GetType())
		{
		case CHARACTER_TYPE::MINJI:
			CreateMinjiBullet();
			break;
		case CHARACTER_TYPE::HANNIE:
			CreateHannieBullet();
			break;
		case CHARACTER_TYPE::DANIELLE:
			CreateDanielleBullet();
			break;
		case CHARACTER_TYPE::HAERIN:
			CreateHaerinBullet();
			break;
		case CHARACTER_TYPE::HYEIN:
			CreateHyeinBullet();
			break;

		default:
			break;
		}
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