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
#include "CCore.h"

CPlayer::CPlayer() : m_bDir(DIR_RIGHT), m_eState(PLAYER_STATE::IDLE), 
					 m_EffectAnimator(nullptr), m_fSpeed(300.f), m_iHP(3), m_iLife(3),
					 m_fDieTime(0.f), m_fResurrectTime(0.f), m_bIsGameOver(false), 
					 m_iKillCount(0), m_iBunnyCount(0), m_iCookieCount(0), m_bPrevDir(DIR_RIGHT),
					 m_vPrevPos(Vec2(0.f, 0.f))
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::update()
{
	PredictPlayerPos();		// ����
	InterpolatePos();		// ����

	PlayAnimation();

	if (m_EffectAnimator != nullptr)
		m_EffectAnimator->update();	// ����Ʈ �ִϸ����� ������Ʈ

	GetAnimator()->update();		// �ִϸ����� ������Ʈ

	// ���� ���� ����
	if (m_bDir != m_bPrevDir)
		m_bPrevDir = m_bDir;

	// ���� ��ġ ����
	if (m_vPrevPos != GetPos())
		m_vPrevPos = GetPos();
}

void CPlayer::render(HDC _dc)
{
	if (m_EffectAnimator != nullptr)
		m_EffectAnimator->render(_dc);

	// ������Ʈ �׸���(�浹ü, �ִϸ��̼�)
	componentRender(_dc);

	int id = CCore::GetInst()->GetID();

	if (GetName() == L"Player" + std::to_wstring(id)) {
		// HP Bar UI �׸���
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

		// Life UI �׸���
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

		// ���ھ� Text
		// ��Ʈ ���� ���
		const TCHAR* fontPath = TEXT("resource\\font\\PixelBook.ttf");

		// ��Ʈ ���ҽ� �߰�
		if (AddFontResourceEx(fontPath, FR_PRIVATE, 0) > 0) {
			// ���� ��� ����
			SetBkMode(_dc, TRANSPARENT);

			// �۾�	�� ����
			SetTextColor(_dc, RGB(0, 0, 0));

			// LOGFONT ����ü ����
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			lstrcpy(lf.lfFaceName, L"Pixel Book");
			lf.lfHeight = 30;  // ��Ʈ ũ�� ����
			lf.lfWeight = 50;  // ��Ʈ �β� ����


			// ��Ʈ ����
			HFONT hFont = CreateFontIndirect(&lf);
			SelectObject(_dc, hFont);

			// �䳢 �� Text ����
			std::wstring str = L"X " + std::to_wstring(m_iBunnyCount);
			TextOut(_dc, 580, 308, str.c_str(), str.length());

			// ��Ű �� Text ����
			str = L"X " + std::to_wstring(m_iCookieCount);
			TextOut(_dc, 580, 338, str.c_str(), str.length());

			// ų �� Text ����
			str = L"X " + std::to_wstring(m_iKillCount);
			TextOut(_dc, 580, 368, str.c_str(), str.length());

			// ��Ʈ ����
			DeleteObject(hFont);
		}

		// ���ҽ� ����
		RemoveFontResourceEx(fontPath, FR_PRIVATE, 0);
	}
}

void CPlayer::CreateBullet(int id)
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Player" + std::to_wstring(id) + L" Bullet");
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
	// Texture �ε��ϱ�
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Minji", L"texture\\characters\\minji\\minji.bmp");

	// Animator �����
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator�� Object ����
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation �����
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

	pAnimator->Play(L"Minji_Idle_Right", true);	// ���� �ִϸ��̼� ����

	SetAnimator(pAnimator);
}

void CPlayer::CreateHannie()
{
	// Texture �ε��ϱ�
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hannie", L"texture\\characters\\hannie\\hannie.bmp");

	// Animator �����
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator�� Object ����
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation �����
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

	pAnimator->Play(L"Hannie_Idle_Right", true);	// ���� �ִϸ��̼� ����

	SetAnimator(pAnimator);
}

void CPlayer::CreateDanielle()
{
	// Texture �ε��ϱ�
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Danielle", L"texture\\characters\\danielle\\danielle.bmp");

	// Animator �����
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator�� Object ����
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation �����
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

	pAnimator->Play(L"Danielle_Idle_Right", true);	// ���� �ִϸ��̼� ����

	SetAnimator(pAnimator);
}

void CPlayer::CreateHyein()
{
	// Texture �ε��ϱ�
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hyein", L"texture\\characters\\hyein\\hyein.bmp");

	// Animator �����
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator�� Object ����
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation �����
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

	pAnimator->Play(L"Hyein_Idle_Right", true);	// ���� �ִϸ��̼� ����

	SetAnimator(pAnimator);
}

void CPlayer::CreateHaerin()
{
	// Texture �ε��ϱ�
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Haerin", L"texture\\characters\\haerin\\hearin.bmp");

	// Animator �����
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator�� Object ����
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation �����
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

	pAnimator->Play(L"Haerin_Idle_Right", true);	// ���� �ִϸ��̼� ����

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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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
				// TODO: HIT Animation �߰�
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

void CPlayer::PredictPlayerPos()
{
	// ������ ������ ����Ͽ� �÷��̾��� ��ġ ����
	Vec2 vPos = GetPos();
	
	// ������ �����ϱ� ���� ����
	float fInterpolate = 10.f;

	if(m_bPrevDir == DIR_LEFT)
		vPos.x -= (m_fSpeed / fInterpolate) * DT;
	else if(m_bPrevDir == DIR_RIGHT)
		vPos.x += (m_fSpeed / fInterpolate) * DT;

	// �� y��ǥ���� ���� y��ǥ�� �� ũ�ٸ�
	if (vPos.y > m_vPrevPos.y)
	{
		vPos.y += (m_fSpeed / fInterpolate) * DT;
	}
	else if (vPos.y < m_vPrevPos.y)
	{
		vPos.y -= (m_fSpeed / fInterpolate) * DT;
	}

	SetPos(vPos);
}

void CPlayer::InterpolatePos()
{
	// ���� ����
	// ���� �����ǰ� ���� ������ ���� ���̸� ����
	Vec2 vPos = GetPos();
	Vec2 vPrevPos = GetPrevPos();
	Vec2 interpolatePos{};

	// ���� ����
	// 0 ~ 1 ������ ��
	float fInterpolate = 0.5f;

	interpolatePos.x = LerpX(vPrevPos.x, vPos.x, fInterpolate);
	interpolatePos.y = LerpY(vPrevPos.y, vPos.y, fInterpolate);

	SetPos(interpolatePos);
}

void CPlayer::PlayShootingEffect()
{
	// ���� ����Ʈ ���
	m_EffectAnimator->Play(L"Shooting", false);
	m_EffectAnimator->FindAnimation(L"Shooting")->SetFrame(0);
}

void CPlayer::OnCollision(CCollider* _pOther)
{
}

void CPlayer::EnterCollision(CCollider* _pOther)
{
}

void CPlayer::ExitCollision(CCollider* _pOther)
{
}
