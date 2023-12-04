#include "pch.h"
#include "CScene_Clear.h"
#include "CCore.h"
#include "CBackground.h"

CScene_Clear::CScene_Clear() : m_bClearAniEnd{ false }
{
}

CScene_Clear::~CScene_Clear()
{
}


void CScene_Clear::update()
{
	CScene::update();
}

void CScene_Clear::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	///////////////////////
	// Background Object //
	///////////////////////
	CObject* pClearAniObj = new CBackground;
	pClearAniObj->SetName(L"Clear Animation");
	pClearAniObj->SetPos(Vec2(0.f, 0.f));
	pClearAniObj->SetScale(Vec2(vResolution.x, vResolution.y));

	// 애니메이터 생성
	pClearAniObj->CreateAnimator();

	CreateObject(pClearAniObj, GROUP_TYPE::BACKGROUND);
	///////////////////////
}

void CScene_Clear::Exit()
{
	DeleteAll();
}

void CScene_Clear::render(HDC _dc)
{
	CScene::render(_dc);
}