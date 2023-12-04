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

CBullet::CBullet() : m_fDir(1.f), m_fSpeed(200.f), m_bIsDown(false), 
					 m_fDegree(0.f), m_vFirstPos(Vec2(0.f, 0.f)), m_vPrevPos(Vec2(0.f, 0.f)),
				     m_fDeadTime(0.f)
{
}

CBullet::CBullet(Vec2 vPos) : m_fDir(1.f), m_fSpeed(200.f), m_bIsDown(false),
							  m_fDegree(0.f), m_vFirstPos(vPos), m_vPrevPos(vPos),
							  m_fDeadTime(0.f)
{
}

CBullet::~CBullet()
{
}

void CBullet::update()
{
	PredictBulletPos();	// 예측
	InterpolatePos();	// 보간

	GetAnimator()->update();

	
	// 이전 위치와 현재 위치가 변화가 없는 시간이 0.1초가 넘으면 존재하지 않는다고 판단
	if ((int)m_vPrevPos.x == (int)GetPos().x) {
		m_fDeadTime += DT;
		if (m_fDeadTime > 0.1f) {
			DeleteObject(this);
		}
	}
	else {
		m_fDeadTime = 0.f;
		m_vPrevPos = GetPos();
	}
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
	case GROUP_TYPE::BULLET_PLAYER: 
	{
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

void CBullet::CreateAnimator(int id)
{
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->FindObject(L"Player" + std::to_wstring(id));

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

void CBullet::PredictBulletPos()
{
	// 마지막 방향을 사용하여 투사체의 위치 예측
	Vec2 vPos = GetPos();

	// 전 x좌표보다 현재 x좌표가 더 크다면
	if ((int)vPos.x > (int)m_vPrevPos.x)
	{
		vPos.x += m_fSpeed * DT;
	}
	else if ((int)vPos.x < (int)m_vPrevPos.x)
	{
		vPos.x -= m_fSpeed * DT;
	}

	// 전 y좌표보다 현재 y좌표가 더 크다면
	if ((int)vPos.y != (int)m_vPrevPos.y) {
		if ((int)vPos.y > (int)m_vPrevPos.y)
		{
			vPos.y += m_fSpeed * DT;
		}
		else if ((int)vPos.y < (int)m_vPrevPos.y)
		{
			vPos.y -= m_fSpeed * DT;
		}
	}

	SetPos(vPos);
}

void CBullet::InterpolatePos()
{
	Vec2 vPos = GetPos();
	Vec2 vPrevPos = m_vPrevPos;

	Vec2 interpolatePos{};

	interpolatePos.x = LerpX(vPrevPos.x, vPos.x, DT * 10.f);
	interpolatePos.y = LerpY(vPrevPos.y, vPos.y, DT * 10.f);

	SetPos(interpolatePos);
}

void CBullet::OnCollision(CCollider* _pOther)
{
}

void CBullet::EnterCollision(CCollider* _pOther)
{
}

void CBullet::ExitCollision(CCollider* _pOther)
{
}