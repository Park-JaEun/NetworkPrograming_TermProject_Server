#include "pch.h"
#include "CUI.h"

#include "CResourceMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CTexture.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Select.h"

CUI::CUI() : m_bOnMouse(false), m_bMousePush(false)
{
}

CUI::~CUI()
{
}


void CUI::update()
{
}

void CUI::render(HDC _dc)
{
	if (GetName() == L"Button Backgound") {
		Vec2 vPos = GetPos();
		Vec2 vScale = GetScale();

		CAnimation* pAnimation = GetAnimator()->FindAnimation(L"Button Background");
		CTexture* pTexture = pAnimation->GetTexture();

		TransparentBlt(_dc,
			(int)vPos.x,
			(int)vPos.y,
			(int)GetScale().x,
			(int)GetScale().y,
			pTexture->GetDC(),
			0,
			0,
			2079,
			779,
			RGB(255, 0, 255));
	}
	else if (GetName() == L"StartButton") {
		Vec2 vPos = GetPos();
		Vec2 vScale = GetScale();
		CAnimation* pAnimation;

		if(GetOnMouse())
			pAnimation = GetAnimator()->FindAnimation(L"StartButton_OnMouse");
		else if(GetMousePush())
			pAnimation = GetAnimator()->FindAnimation(L"StartButton_Push");
		else
			pAnimation = GetAnimator()->FindAnimation(L"StartButton_Idle");

		CTexture* pTexture = pAnimation->GetTexture();

		TransparentBlt(_dc,
						(int)vPos.x,
						(int)vPos.y,
						(int)GetScale().x,
						(int)GetScale().y,
						pTexture->GetDC(),
						0,
						0,
						1060,
						346,
						RGB(255, 0, 255));
	}
	else if (GetName() == L"ExitButton") {
		Vec2 vPos = GetPos();
		Vec2 vScale = GetScale();

		CAnimation* pAnimation = GetAnimator()->FindAnimation(L"ExitButton_Idle");
		CTexture* pTexture = pAnimation->GetTexture();

		TransparentBlt(_dc,
						(int)vPos.x,
						(int)vPos.y,
						(int)GetScale().x,
						(int)GetScale().y,
						pTexture->GetDC(),
						0,
						0,
						1060,
						346,
						RGB(255, 0, 255));
	}
	else if (GetName() == L"IPInput") {
		Vec2 vPos = GetPos();
		Vec2 vScale = GetScale();

		CAnimation* pAnimation = GetAnimator()->FindAnimation(L"IPInput");
		CTexture* pTexture = pAnimation->GetTexture();

		TransparentBlt(_dc,
						(int)vPos.x,
						(int)vPos.y,
						(int)GetScale().x,
						(int)GetScale().y,
						pTexture->GetDC(),
						0,
						0,
						2637,
						370,
						RGB(255, 0, 255));
	}
	else if (GetName() == L"NicknameInput") {
		Vec2 vPos = GetPos();
		Vec2 vScale = GetScale();

		CAnimation* pAnimation = GetAnimator()->FindAnimation(L"NicknameInput");
		CTexture* pTexture = pAnimation->GetTexture();

		TransparentBlt(_dc,
						(int)vPos.x,
						(int)vPos.y,
						(int)GetScale().x,
						(int)GetScale().y,
						pTexture->GetDC(),
						0,
						0,
						2637,
						368,
						RGB(255, 0, 255));
	}
	else if (GetName() == L"Selected Character") {
		CAnimation* pAnimation{};
		CTexture* pTexture{};

		switch (((CScene_Select*)CSceneMgr::GetInst()->GetCurScene())->GetSelectedCharacter()) {
		case CHARACTER_TYPE::MINJI:
			pAnimation = GetAnimator()->FindAnimation(L"Minji Selected");
			break;
		case CHARACTER_TYPE::DANIELLE:
			pAnimation = GetAnimator()->FindAnimation(L"Danielle Selected");
			break;
		case CHARACTER_TYPE::HAERIN:
			pAnimation = GetAnimator()->FindAnimation(L"Haerin Selected");
			break;
		case CHARACTER_TYPE::HANNIE:
			pAnimation = GetAnimator()->FindAnimation(L"Hannie Selected");
			break;
		case CHARACTER_TYPE::HYEIN:
			pAnimation = GetAnimator()->FindAnimation(L"Hyein Selected");
			break;
		}

		pTexture = pAnimation->GetTexture();

		TransparentBlt(_dc,
						0,
						0,
						(int)GetScale().x,
						(int)GetScale().y,
						pTexture->GetDC(),
						0,
						0,
						1600,
						900,
						RGB(255, 0, 255));
	}
}

void CUI::CreateAnimator()
{
	if (GetName() == L"Button Backgound") {
		// Texture 로딩하기
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Button Background", L"texture\\ui\\Button_Background.bmp");
	
		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"Button Background",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(2079.f, 779.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->Play(L"Button Background", true);	// 현재 애니메이션 지정

		SetAnimator(pAnimator);
	}
	else if (GetName() == L"StartButton") {
		// Texture 로딩하기
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"StartButton", L"texture\\ui\\StartButton.bmp");

		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"StartButton_Idle",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(1060.f, 346.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"StartButton_OnMouse",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 346.f),			// Texture Left Top
			Vec2(1060.f, 346.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"StartButton_Push",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 692.f),			// Texture Left Top
			Vec2(1060.f, 346.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		SetAnimator(pAnimator);
	}
	else if (GetName() == L"ExitButton") {
		// Texture 로딩하기
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"ExitButton", L"texture\\ui\\ExitButton.bmp");

		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"ExitButton_Idle",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(1060.f, 346.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"ExitButton_OnMouse",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 346.f),			// Texture Left Top
			Vec2(1060.f, 346.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"ExitButton_Push",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 692.f),			// Texture Left Top
			Vec2(1060.f, 346.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		SetAnimator(pAnimator);
	}
	else if (GetName() == L"IPInput") {
		// Texture 로딩하기
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"IPInput", L"texture\\ui\\IP.bmp");

		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"IPInput",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(2637.f, 370.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		SetAnimator(pAnimator);
	}
	else if (GetName() == L"NicknameInput") {
		// Texture 로딩하기
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"NicknameInput", L"texture\\ui\\Nickname.bmp");

		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"NicknameInput",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(2637.f, 368.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		SetAnimator(pAnimator);
	}
	else if (GetName() == L"Selected Character") {
		// Texture 로딩하기
		CTexture* pTextureMinji		= CResourceMgr::GetInst()->LoadTexture(L"Minji Selected", L"texture\\ui\\minji.bmp");
		CTexture* pTextureDanielle	= CResourceMgr::GetInst()->LoadTexture(L"Danielle Selected", L"texture\\ui\\danielle.bmp");
		CTexture* pTextureHaerin	= CResourceMgr::GetInst()->LoadTexture(L"Haerin Selected", L"texture\\ui\\haerin.bmp");
		CTexture* pTextureHannie	= CResourceMgr::GetInst()->LoadTexture(L"Hannie Selected", L"texture\\ui\\hannie.bmp");
		CTexture* pTextureHyein		= CResourceMgr::GetInst()->LoadTexture(L"Hyein Selected", L"texture\\ui\\hyein.bmp");

		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"Minji Selected",			// Animation Name
			pTextureMinji,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(1600.f, 900.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"Danielle Selected",			// Animation Name
			pTextureDanielle,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(1600.f, 900.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"Haerin Selected",			// Animation Name
			pTextureHaerin,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(1600.f, 900.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"Hannie Selected",			// Animation Name
			pTextureHannie,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(1600.f, 900.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->CreateAnimation(
			L"Hyein Selected",			// Animation Name
			pTextureHyein,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(1600.f, 900.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		SetAnimator(pAnimator);
	}
}
