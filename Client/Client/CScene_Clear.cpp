#include "pch.h"
#include "CScene_Clear.h"
#include "CCore.h"
#include "CBackground.h"

CScene_Clear::CScene_Clear()
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
	CObject* pBGObj = new CBackground;
	pBGObj->SetName(L"Clear Background");
	pBGObj->SetPos(Vec2(0.f, 0.f));
	pBGObj->SetScale(Vec2(vResolution.x, vResolution.y));

	// 애니메이터 생성
	pBGObj->CreateAnimator();

	CreateObject(pBGObj, GROUP_TYPE::BACKGROUND);
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