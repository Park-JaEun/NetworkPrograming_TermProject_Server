#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"

#include "CKeyMgr.h"
#include "CTimer.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Main.h"

#include "CResourceMgr.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CAnimation.h"

CPlayer::CPlayer() : m_bDir(DIR_RIGHT), m_eState(PLAYER_STATE::IDLE), 
					 m_EffectAnimator(nullptr), m_fSpeed(300.f), m_iHP(3), m_iLife(1),
					 m_fDieTime(0.f), m_fResurrectTime(0.f), m_bIsGameOver(false)
{
}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	// 게임 오버시 return
	if (m_bIsGameOver)
		return;

	Vec2 vPos = GetPos();
	Vec2 vDummyPos{};
	CScene_Main* pMainScene = (CScene_Main*)CSceneMgr::GetInst()->GetCurScene();
	bool bIsBoss = pMainScene->GetIsBoss();
	
	if (m_iHP <= 0) {
		if (m_eState != PLAYER_STATE::DIE) {
			m_eState = PLAYER_STATE::DIE;
		}

		// 1.5초뒤 아래로 추락
		if (m_fDieTime >= 1.5f && m_fDieTime <= 4.5)
			vPos.y += DT * 50.f * 3;
		else
			m_fDieTime += DT;

		// 3초뒤 부활
		if (!m_bIsGameOver && m_iLife >= 1 && m_fResurrectTime >= 3.0f && m_fDieTime >= 1.5f) {
			m_iLife -= 1;
			if (m_iLife == 0) {
				m_bIsGameOver = true;
				return;
			}
			m_iHP = 3;
			m_fDieTime = 0.f;
			m_fResurrectTime = 0.f;
			m_eState = PLAYER_STATE::IDLE;
			vPos.y = 0.f;
		}
		else if (m_fDieTime >= 1.5f) {
			m_fResurrectTime += DT;
		}
	}
	else {
		// 상
		if (KEY_HOLD(KEY::UP)) {
			vDummyPos = Vec2(vPos.x, vPos.y - m_fSpeed * DT);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.y -= m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.y -= m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
		}
		if (KEY_AWAY(KEY::UP)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 하
		if (KEY_HOLD(KEY::DOWN)) {
			vDummyPos = Vec2(vPos.x, vPos.y + m_fSpeed * DT);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.y += m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.y += m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
		}
		if (KEY_AWAY(KEY::DOWN)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 좌
		if (KEY_HOLD(KEY::LEFT)) {
			vDummyPos = Vec2(vPos.x - m_fSpeed * DT, vPos.y);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.x -= m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.x -= m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
			if (m_bDir != DIR_LEFT)
				m_bDir = DIR_LEFT;
		}
		if (KEY_AWAY(KEY::LEFT)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 우
		if (KEY_HOLD(KEY::RIGHT)) {
			vDummyPos = Vec2(vPos.x + m_fSpeed * DT, vPos.y);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.x += m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.x += m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
			if (m_bDir != DIR_RIGHT)
				m_bDir = DIR_RIGHT;
		}
		if (KEY_AWAY(KEY::RIGHT)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		if (KEY_TAP(KEY::SPACE)) {
			// 총알 발사
			CreateBullet();

			// 슈팅 이펙트 재생
			m_EffectAnimator->Play(L"Shooting", false);
			m_EffectAnimator->FindAnimation(L"Shooting")->SetFrame(0);
		}
	}

	PlayAnimation();

	if (m_EffectAnimator != nullptr)
		m_EffectAnimator->update();

	SetPos(vPos);				// 위치 업데이트
	GetAnimator()->update();	// 애니메이터 업데이트
}

void CPlayer::render(HDC _dc)
{
	if (m_EffectAnimator != nullptr)
		m_EffectAnimator->render(_dc);

	// HP Bar UI 그리기
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"HP", L"texture\\ui\\hp.bmp");
	for (int i = 0; i < m_iHP; ++i) {
		TransparentBlt(_dc,
			24 + 18 * i,
			382,
			18,
			18,
			pTexture->GetDC(),
			0,
			9,
			12,
			9,
			RGB(255, 0, 255));
	}

	// Life UI 그리기
	pTexture = CResourceMgr::GetInst()->LoadTexture(L"Life", L"texture\\ui\\life.bmp");
	for (int i = 0; i < m_iLife; ++i) {
		TransparentBlt(_dc,
			26 * i,
			20,
			26,
			20,
			pTexture->GetDC(),
			0,
			10,
			13,
			10,
			RGB(255, 0, 255));
	}

	// 컴포넌트 그리기(충돌체, 애니메이션)
	componentRender(_dc);
}

void CPlayer::CreateBullet()
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Player Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetFirstPos(vBulletPos);
	pBullet->SetScale(Vec2(14.f, 13.f));
	
	if (m_bDir == DIR_RIGHT)
		pBullet->SetDir(DIR_RIGHT);
	else
		pBullet->SetDir(DIR_LEFT);

	pBullet->CreateCollider();
	pBullet->CreateAnimator(GROUP_TYPE::BULLET_PLAYER);

	pBullet->SetSpeed(700.f);
	pBullet->GetCollider()->SetScale(Vec2(25.f, 25.f));

	CreateObject(pBullet, GROUP_TYPE::BULLET_PLAYER);
}

void CPlayer::CreateAnimator()
{
	switch (m_eType)
	{
	case CHARACTER_TYPE::MINJI:
		CreateMinji();
		break;
	case CHARACTER_TYPE::HANNIE:
		CreateHannie();
		break;
	case CHARACTER_TYPE::DANIELLE:
		CreateDanielle();
		break;
	case CHARACTER_TYPE::HAERIN:
		CreateHaerin();
		break;
	case CHARACTER_TYPE::HYEIN:
		CreateHyein();
		break;

	default:
		break;
	}
}

void CPlayer::CreateMinji()
{
	// Texture 로딩하기
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Minji", L"texture\\characters\\minji\\minji.bmp");

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Minji_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(24.f, 33.f),			// Frame Size
		Vec2(0.f, 0.f),				// Step
		0.f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		1);							// Frame Count

	pAnimator->CreateAnimation(
		L"Minji_Idle_Left",
		pTexture,
		Vec2(0.f, 97.f),
		Vec2(24.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Minji_Run_Right",
		pTexture,
		Vec2(0.f, 65.f),
		Vec2(29.f, 32.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Minji_Run_Left",
		pTexture,
		Vec2(0.f, 162.f),
		Vec2(29.f, 32.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Minji_Die_Right",
		pTexture,
		Vec2(185.f, 0.f),
		Vec2(24.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Minji_Die_Left",
		pTexture,
		Vec2(185.f, 33.f),
		Vec2(24.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);


	m_EffectAnimator->CreateAnimation(
		L"Shooting",			// Animation Name
		pTexture,				// Texture Pointer
		Vec2(0.f, 194.f),		// Texture Left Top
		Vec2(36.f, 35.f),		// Frame Size
		Vec2(36.f, 0.f),		// Step
		0.07f,					// Duration
		Vec2(0.f, 0.f),			// Offset
		Vec2(2.f, 2.f),			// Scale
		7);						// Frame Count

	pAnimator->Play(L"Minji_Idle_Right", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CPlayer::CreateHannie()
{
	// Texture 로딩하기
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hannie", L"texture\\characters\\hannie\\hannie.bmp");

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Hannie_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(43.f, 34.f),			// Frame Size
		Vec2(0.f, 0.f),				// Step
		0.f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		1);							// Frame Count

	pAnimator->CreateAnimation(
		L"Hannie_Idle_Left",
		pTexture,
		Vec2(0.f, 100.f),
		Vec2(43.f, 34.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hannie_Run_Right",
		pTexture,
		Vec2(0.f, 67.f),
		Vec2(43.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hannie_Run_Left",
		pTexture,
		Vec2(0.f, 167.f),
		Vec2(43.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hannie_Die_Right",
		pTexture,
		Vec2(218.f, 0.f),
		Vec2(43.f, 34.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hannie_Die_Left",
		pTexture,
		Vec2(218.f, 34.f),
		Vec2(43.f, 34.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);


	m_EffectAnimator->CreateAnimation(
		L"Shooting",			// Animation Name
		pTexture,				// Texture Pointer
		Vec2(0.f, 200.f),		// Texture Left Top
		Vec2(36.f, 35.f),		// Frame Size
		Vec2(36.f, 0.f),		// Step
		0.07f,					// Duration
		Vec2(0.f, 0.f),			// Offset
		Vec2(2.f, 2.f),			// Scale
		7);						// Frame Count

	pAnimator->Play(L"Hannie_Idle_Right", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CPlayer::CreateDanielle()
{
	// Texture 로딩하기
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Danielle", L"texture\\characters\\danielle\\danielle.bmp");

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Danielle_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(34.f, 33.f),			// Frame Size
		Vec2(0.f, 0.f),				// Step
		0.f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		1);							// Frame Count

	pAnimator->CreateAnimation(
		L"Danielle_Idle_Left",
		pTexture,
		Vec2(0.f, 97.f),
		Vec2(34.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Danielle_Run_Right",
		pTexture,
		Vec2(0.f, 65.f),
		Vec2(35.f, 32.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Danielle_Run_Left",
		pTexture,
		Vec2(0.f, 162.f),
		Vec2(35.f, 32.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Danielle_Die_Right",
		pTexture,
		Vec2(187.f, 0.f),
		Vec2(34.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Danielle_Die_Left",
		pTexture,
		Vec2(187.f, 33.f),
		Vec2(34.f, 33.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);


	m_EffectAnimator->CreateAnimation(
		L"Shooting",			// Animation Name
		pTexture,				// Texture Pointer
		Vec2(0.f, 194.f),		// Texture Left Top
		Vec2(36.f, 35.f),		// Frame Size
		Vec2(36.f, 0.f),		// Step
		0.07f,					// Duration
		Vec2(0.f, 0.f),			// Offset
		Vec2(2.f, 2.f),			// Scale
		7);						// Frame Count

	pAnimator->Play(L"Danielle_Idle_Right", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CPlayer::CreateHyein()
{
	// Texture 로딩하기
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hyein", L"texture\\characters\\hyein\\hyein.bmp");

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Hyein_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(38.f, 32.f),			// Frame Size
		Vec2(0.f, 0.f),				// Step
		0.f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		1);							// Frame Count

	pAnimator->CreateAnimation(
		L"Hyein_Idle_Left",
		pTexture,
		Vec2(0.f, 94.f),
		Vec2(38.f, 32.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hyein_Run_Right",
		pTexture,
		Vec2(0.f, 63.f),
		Vec2(37.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hyein_Run_Left",
		pTexture,
		Vec2(0.f, 157.f),
		Vec2(37.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hyein_Die_Right",
		pTexture,
		Vec2(210.f, 0.f),
		Vec2(38.f, 32.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Hyein_Die_Left",
		pTexture,
		Vec2(210.f, 32.f),
		Vec2(38.f, 32.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);


	m_EffectAnimator->CreateAnimation(
		L"Shooting",			// Animation Name
		pTexture,				// Texture Pointer
		Vec2(0.f, 188.f),		// Texture Left Top
		Vec2(36.f, 35.f),		// Frame Size
		Vec2(36.f, 0.f),		// Step
		0.07f,					// Duration
		Vec2(0.f, 0.f),			// Offset
		Vec2(2.f, 2.f),			// Scale
		7);						// Frame Count

	pAnimator->Play(L"Hyein_Idle_Right", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CPlayer::CreateHaerin()
{
	// Texture 로딩하기
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Haerin", L"texture\\characters\\haerin\\hearin.bmp");

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Haerin_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(27.f, 31.f),			// Frame Size
		Vec2(0.f, 0.f),				// Step
		0.f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		1);							// Frame Count

	pAnimator->CreateAnimation(
		L"Haerin_Idle_Left",
		pTexture,
		Vec2(0.f, 91.f),
		Vec2(27.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Haerin_Run_Right",
		pTexture,
		Vec2(0.f, 61.f),
		Vec2(31.f, 30.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Haerin_Run_Left",
		pTexture,
		Vec2(0.f, 152.f),
		Vec2(31.f, 30.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Haerin_Die_Right",
		pTexture,
		Vec2(187.f, 0.f),
		Vec2(27.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Haerin_Die_Left",
		pTexture,
		Vec2(187.f, 31.f),
		Vec2(27.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);


	m_EffectAnimator->CreateAnimation(
		L"Shooting",		// Animation Name
		pTexture,				// Texture Pointer
		Vec2(0.f, 182.f),		// Texture Left Top
		Vec2(36.f, 35.f),		// Frame Size
		Vec2(36.f, 0.f),		// Step
		0.07f,					// Duration
		Vec2(0.f, 0.f),			// Offset
		Vec2(2.f, 2.f),			// Scale
		7);						// Frame Count

	pAnimator->Play(L"Haerin_Idle_Right", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CPlayer::PlayAnimation()
{
	switch (m_eType)
	{
	case CHARACTER_TYPE::MINJI: 
	{
		if (m_bDir == DIR_RIGHT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Minji_Idle_Right", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Minji_Run_Right", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Minji_Die_Right", true);
				break;
			}
		}
		else if (m_bDir == DIR_LEFT) 
		{
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Minji_Idle_Left", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Minji_Run_Left", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Minji_Die_Left", true);
				break;
			}
		}
	}
		break;
	case CHARACTER_TYPE::HANNIE: 
	{
		if (m_bDir == DIR_RIGHT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Hannie_Idle_Right", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Hannie_Run_Right", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Hannie_Die_Right", true);
				break;
			}
		}
		else if (m_bDir == DIR_LEFT) 
		{
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Hannie_Idle_Left", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Hannie_Run_Left", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Hannie_Die_Left", true);
				break;
			}
		}
	}
		break;
	case CHARACTER_TYPE::DANIELLE: 
	{
		if (m_bDir == DIR_RIGHT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Danielle_Idle_Right", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Danielle_Run_Right", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Danielle_Die_Right", true);
				break;
			}
		}
		else if (m_bDir == DIR_LEFT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Danielle_Idle_Left", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Danielle_Run_Left", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Danielle_Die_Left", true);
				break;
			}
		}
	}
		break;
	case CHARACTER_TYPE::HAERIN: 
	{
		if (m_bDir == DIR_RIGHT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Haerin_Idle_Right", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Haerin_Run_Right", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Haerin_Die_Right", true);
				break;
			}
		}
		else if (m_bDir == DIR_LEFT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Haerin_Idle_Left", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Haerin_Run_Left", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Haerin_Die_Left", true);
				break;
			}
		}
	}
		break;
	case CHARACTER_TYPE::HYEIN: 
	{
		if (m_bDir == DIR_RIGHT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Hyein_Idle_Right", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Hyein_Run_Right", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Hyein_Die_Right", true);
				break;
			}
		}
		else if (m_bDir == DIR_LEFT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Hyein_Idle_Left", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Hyein_Run_Left", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			case PLAYER_STATE::DIE:
				GetAnimator()->Play(L"Hyein_Die_Left", true);
				break;
			}
		}
	}
		break;
	default:
		break;
	}

	
}

void CPlayer::OnCollision(CCollider* _pOther)
{
}

void CPlayer::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster Bullet") {
		if(m_iHP > 0)
			--m_iHP;
	}

	if (pOtherObj->GetName() == L"Boss Bullet" || pOtherObj->GetName() == L"Boss Missile") {
		if (m_iHP > 0)
			--m_iHP;
	}
}

void CPlayer::ExitCollision(CCollider* _pOther)
{
}
