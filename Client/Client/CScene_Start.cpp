#include "pch.h"
#include "CScene_Start.h"
#include "CKeyMgr.h"

#include "CCore.h"
#include "CBackground.h"

#include "CUI.h"
#include "CCollider.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	///////////////////////
	// Background Object //
	///////////////////////
	CObject* pBGObj = new CBackground;
	pBGObj->SetName(L"Start Background");
	pBGObj->SetPos(Vec2(0.f, 0.f));
	pBGObj->SetScale(Vec2(vResolution.x, vResolution.y));

	// 애니메이터 생성
	pBGObj->CreateAnimator();

	CreateObject(pBGObj, GROUP_TYPE::BACKGROUND);
	///////////////////////

	///////////////
	// UI Object //
	///////////////

	CUI* pButtonBackgroundUI = new CUI;
	pButtonBackgroundUI->SetName(L"Button Backgound");
	pButtonBackgroundUI->SetScale(Vec2(320.f, 140.f));
	pButtonBackgroundUI->SetPos(Vec2(160.f, 250.f));
	pButtonBackgroundUI->CreateAnimator();
	AddObject(pButtonBackgroundUI, GROUP_TYPE::UI);

	CUI* pStartButtonUI = new CUI;
	pStartButtonUI->SetName(L"StartButton");
	pStartButtonUI->SetScale(Vec2(90.f, 30.f));
	pStartButtonUI->SetPos(Vec2(210.f, 352.f));
	pStartButtonUI->CreateAnimator();
	pStartButtonUI->CreateCollider();
	pStartButtonUI->GetCollider()->SetScale(Vec2(90.f, 30.f));
	AddObject(pStartButtonUI, GROUP_TYPE::UI);

	CUI* pExitButtonUI = new CUI;
	pExitButtonUI->SetName(L"ExitButton");
	pExitButtonUI->SetScale(Vec2(90.f, 30.f));
	pExitButtonUI->SetPos(Vec2(340.f, 352.f));
	pExitButtonUI->CreateAnimator();
	pExitButtonUI->CreateCollider();
	pExitButtonUI->GetCollider()->SetScale(Vec2(90.f, 30.f));
	AddObject(pExitButtonUI, GROUP_TYPE::UI);

	///////////////


}

void CScene_Start::Exit()
{
	DeleteAll();
}

void CScene_Start::update()
{
	CScene::update();

	


	if (KEY_TAP(KEY::LBOTTON)) {
		
		// 마우스 좌표를 받아온다.
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

		// 마우스가 StartButton 위에 있는지 확인한다.
		CUI* pStartButton = (CUI*)FindObject(L"StartButton");

		if (pStartButton->GetCollider()->PtInCollider(vMousePos))
			ChangeScene(SCENE_TYPE::MAIN);
			//pStartButton->SetOnMouse(true);
		//else
			//pStartButton->SetOnMouse(false);
		
		
	}
}