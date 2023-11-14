#include "pch.h"
#include "CUI.h"

#include "CResourceMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CTexture.h"

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
}
