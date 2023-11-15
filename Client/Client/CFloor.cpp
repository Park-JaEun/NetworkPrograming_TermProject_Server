#include "pch.h"
#include "CFloor.h"

#include "CResourceMgr.h"
#include "CAnimator.h"

#include "CTexture.h"

CFloor::CFloor()
{
}

CFloor::~CFloor()
{
}

void CFloor::update()
{
}

void CFloor::render(HDC _dc)
{
	componentRender(_dc);
}

void CFloor::CreateAnimator()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Log", L"texture\\background\\log.bmp");

	// Animator �����
	CAnimator* pAnimator = new CAnimator;

	// Animator�� Object ����
	pAnimator->SetObj(this);

	// Animation �����
	pAnimator->CreateAnimation(
		L"Log",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(210.f, 50.f),			// Frame Size
		Vec2(0.f, 0.f),			// Step
		0.0f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		1);							// Frame Count

	pAnimator->Play(L"Log", true);	// ���� �ִϸ��̼� ����

	SetAnimator(pAnimator);
}