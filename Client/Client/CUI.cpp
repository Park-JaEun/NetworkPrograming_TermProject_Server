#include "pch.h"
#include "CUI.h"

#include "CResourceMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CTexture.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Select.h"

CUI::CUI() : m_bOnMouse{false}, m_bMousePush{ false }
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
		CTexture* pTexture{};

		switch (((CScene_Select*)CSceneMgr::GetInst()->GetCurScene())->GetSelectedCharacter()) 
		{
		case CHARACTER_TYPE::MINJI:
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Minji Selected", L"texture\\ui\\minji.bmp");
			break;
		case CHARACTER_TYPE::DANIELLE:
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Danielle Selected", L"texture\\ui\\danielle.bmp");
			break;
		case CHARACTER_TYPE::HAERIN:
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Haerin Selected", L"texture\\ui\\haerin.bmp");
			break;
		case CHARACTER_TYPE::HANNIE:
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hannie Selected", L"texture\\ui\\hannie.bmp");
			break;
		case CHARACTER_TYPE::HYEIN:
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hyein Selected", L"texture\\ui\\hyein.bmp");
			break;
		}

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
	else if (GetName() == L"Profile") {
		CTexture* pTexture{};

		switch (((CScene_Select*)CSceneMgr::GetInst()->GetCurScene())->GetSelectedCharacter())
		{
		case CHARACTER_TYPE::MINJI:
		{
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Minji", L"texture\\characters\\minji\\minji.bmp");

			TransparentBlt(_dc,
				3,
				312,
				70,
				70,
				pTexture->GetDC(),
				185,
				66,
				70,
				70,
				RGB(255, 0, 255));
		}
			break;
		case CHARACTER_TYPE::DANIELLE:
		{
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Danielle", L"texture\\characters\\danielle\\danielle.bmp");
			TransparentBlt(_dc,
				10,
				314,
				70,
				70,
				pTexture->GetDC(),
				187,
				66,
				70,
				70,
				RGB(255, 0, 255));
		}
			break;
		case CHARACTER_TYPE::HAERIN:
		{
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Haerin", L"texture\\characters\\haerin\\haerin.bmp");
			TransparentBlt(_dc,
				3,
				312,
				70,
				70,
				pTexture->GetDC(),
				187,
				62,
				70,
				70,
				RGB(255, 0, 255));
		}
			break;
		case CHARACTER_TYPE::HANNIE:
		{
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hannie", L"texture\\characters\\hannie\\hannie.bmp");
			TransparentBlt(_dc,
				3,
				316,
				70,
				70,
				pTexture->GetDC(),
				218,
				68,
				70,
				70,
				RGB(255, 0, 255));
		}
			break;
		case CHARACTER_TYPE::HYEIN:
		{
			pTexture = CResourceMgr::GetInst()->LoadTexture(L"Hyein", L"texture\\characters\\hyein\\hyein.bmp");
			TransparentBlt(_dc,
				3,
				312,
				70,
				70,
				pTexture->GetDC(),
				210,
				64,
				70,
				70,
				RGB(255, 0, 255));
		}
			break;
		}
	}
	else if (GetName() == L"HP") {
		CTexture*  pTexture = CResourceMgr::GetInst()->LoadTexture(L"HP", L"texture\\ui\\hp.bmp");
		TransparentBlt(_dc,
			0,
			382,
			24,
			18,
			pTexture->GetDC(),
			0,
			0,
			12,
			9,
			RGB(255, 0, 255));
	}
	else if (GetName() == L"HP Bar") {
		/*CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"HP", L"texture\\ui\\hp.bmp");
		TransparentBlt(_dc,
			(int)GetPos().x,
			(int)GetPos().y,
			18,
			18,
			pTexture->GetDC(),
			0,
			9,
			12,
			9,
			RGB(255, 0, 255));*/
	}
	else if (GetName() == L"Life Text") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Life", L"texture\\ui\\life.bmp");
		TransparentBlt(_dc,
			0,
			0,
			68,
			20,
			pTexture->GetDC(),
			0,
			0,
			34,
			10,
			RGB(255, 0, 255));
	}
	else if (GetName() == L"Score Text") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Score", L"texture\\ui\\itemUI.bmp");
		TransparentBlt(_dc,
			546,
			280,
			94,
			26,
			pTexture->GetDC(),
			0,
			0,
			47,
			13,
			RGB(255, 0, 255));
	}
	else if (GetName() == L"Bunny Score") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Score", L"texture\\ui\\itemUI.bmp");
		TransparentBlt(_dc,
						546,
						310,
						26,
						24,
						pTexture->GetDC(),
						0,
						13,
						13,
						12,
						RGB(255, 0, 255));
	}
	else if (GetName() == L"Cookie Score") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Score", L"texture\\ui\\itemUI.bmp");
		TransparentBlt(_dc,
						548,
						339,
						22,
						24,
						pTexture->GetDC(),
						0,
						25,
						11,
						12,
						RGB(255, 0, 255));
	}
	else if (GetName() == L"Kill Score") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Score", L"texture\\ui\\itemUI.bmp");
		TransparentBlt(_dc,
						548,
						368,
						21,
						27,
						pTexture->GetDC(),
						0,
						37,
						7,
						9,
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
	}
	else if (GetName() == L"HP") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"HP", L"texture\\ui\\hp.bmp");
	}
	else if (GetName() == L"Life Text") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Life", L"texture\\ui\\life.bmp");
	}
	else if (GetName() == L"Score Text") {
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Score", L"texture\\ui\\itemUI.bmp");
	}
}
