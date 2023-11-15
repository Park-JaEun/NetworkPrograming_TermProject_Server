#include "pch.h"
#include "CBackground.h"

#include "CResourceMgr.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CTimer.h"

CBackground::CBackground()
{
}

CBackground::~CBackground()
{
}

void CBackground::update()
{
}

void CBackground::render(HDC _dc)
{
	if (GetName() == L"Start Background") {
		static float fDelay = 0.f;
		static int iCurFrame = 0;

		if (fDelay > 0.05f) {
			++iCurFrame;
			fDelay = 0.f;
		}

		CAnimation* pAnimation = GetAnimator()->FindAnimation(L"Start Background");
		CTexture* pTexture = pAnimation->GetTexture();

		TransparentBlt(_dc,
			0,
			0,
			(int)GetScale().x,
			(int)GetScale().y,
			pTexture->GetDC(),
			0,
			0 + (720 * iCurFrame),
			1280,
			720,
			RGB(255, 0, 255));

		if (iCurFrame == 22)
			iCurFrame = 0;

		fDelay += DT;
		
	}
	else if (GetName() == L"Main Background") {
		CAnimation* pAnimation = GetAnimator()->FindAnimation(L"Main Background");
		CTexture* pTexture = pAnimation->GetTexture();

		// 카메라 좌표를 받아옴
		Vec2 vPos = CCamera::GetInst()->GetLookAt();

		// 카메라 영역에서 배경을 그러줌
		TransparentBlt(_dc,
			0,
			0,
			(int)GetScale().x,
			(int)GetScale().y,
			pTexture->GetDC(),
			(int)vPos.x,
			(int)0,
			(int)(pTexture->GetWidth() / 8.f),
			(int)pTexture->GetHeight(),
			RGB(255, 0, 255));
	}
	
}

void CBackground::CreateAnimator()
{
	if (GetName() == L"Start Background") {
		// Texture 로딩하기
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Start Background Texture", L"texture\\background\\START2.bmp");

		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"Start Background",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(1.f, 1.f),			// Texture Left Top
			Vec2(1036.f, 582.f),		// Frame Size
			Vec2(0.f, 584.f),			// Step
			0.01f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			41);						// Frame Count

		pAnimator->Play(L"Start Background", true);	// 현재 애니메이션 지정

		SetAnimator(pAnimator);
	}
	else if (GetName() == L"Main Background") {
		// Texture 로딩하기
		CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Main Background Texture", L"texture\\background\\city.bmp");

		// Animator 만들기
		CAnimator* pAnimator = new CAnimator;

		pAnimator->SetObj(this);

		pAnimator->CreateAnimation(
			L"Main Background",			// Animation Name
			pTexture,				// Texture Pointer
			Vec2(0.f, 0.f),			// Texture Left Top
			Vec2(724.f, 407.f),		// Frame Size
			Vec2(0.f, 0.f),			// Step
			0.0f,					// Duration
			Vec2(0.f, 0.f),			// Offset
			1);						// Frame Count

		pAnimator->Play(L"Main Background", true);	// 현재 애니메이션 지정

		SetAnimator(pAnimator);
	}
}
