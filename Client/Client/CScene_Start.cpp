#include "pch.h"
#include "CScene_Start.h"
#include "CKeyMgr.h"

#include "CCore.h"
#include "CBackground.h"

#include "CUI.h"
#include "CCollider.h"
#include "CSound.h"
#include "CResourceMgr.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	HINSTANCE hInst{};
	HWND hWnd = CCore::GetInst()->GetMainHwnd();
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	// BGM Ʋ��
	CSoundMgr::GetInst()->init();
	CResourceMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\NewJeans.wav");
	CSound* pSound = CResourceMgr::GetInst()->FindSound(L"BGM_01");
	pSound->PlayToBGM();

	///////////////////////
	// Background Object //
	///////////////////////
	CObject* pBGObj = new CBackground;
	pBGObj->SetName(L"Start Background");
	pBGObj->SetPos(Vec2(0.f, 0.f));
	pBGObj->SetScale(Vec2(vResolution.x, vResolution.y));

	// �ִϸ����� ����
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
	pStartButtonUI->SetScale(Vec2(100.f, 30.f));
	pStartButtonUI->SetPos(Vec2(205.f, 352.f));
	pStartButtonUI->CreateAnimator();
	pStartButtonUI->CreateCollider();
	pStartButtonUI->GetCollider()->SetScale(Vec2(90.f, 30.f));
	AddObject(pStartButtonUI, GROUP_TYPE::UI);

	CUI* pExitButtonUI = new CUI;
	pExitButtonUI->SetName(L"ExitButton");
	pExitButtonUI->SetScale(Vec2(100.f, 30.f));
	pExitButtonUI->SetPos(Vec2(335.f, 352.f));
	pExitButtonUI->CreateAnimator();
	pExitButtonUI->CreateCollider();
	pExitButtonUI->GetCollider()->SetScale(Vec2(90.f, 30.f));
	AddObject(pExitButtonUI, GROUP_TYPE::UI);

	CUI* pIPInputUI = new CUI;
	pIPInputUI->SetName(L"IPInput");
	pIPInputUI->SetScale(Vec2(260.f, 35.f));
	pIPInputUI->SetPos(Vec2(190.f, 305.f));
	pIPInputUI->CreateAnimator();
	pIPInputUI->CreateCollider();
	pIPInputUI->GetCollider()->SetScale(Vec2(90.f, 30.f));
	AddObject(pIPInputUI, GROUP_TYPE::UI);

	CUI* pNicknameInputUI = new CUI;
	pNicknameInputUI->SetName(L"NicknameInput");
	pNicknameInputUI->SetScale(Vec2(260.f, 35.f));
	pNicknameInputUI->SetPos(Vec2(190.f, 265.f));
	pNicknameInputUI->CreateAnimator();
	pNicknameInputUI->CreateCollider();
	pNicknameInputUI->GetCollider()->SetScale(Vec2(90.f, 30.f));
	AddObject(pNicknameInputUI, GROUP_TYPE::UI);

	// �г��� �Է��� ���� Edit ��Ʈ��
	hEditNickname = CreateWindow(L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		299, 271, 140, 35, // x, y, width, height
		hWnd, (HMENU)100, hInst, NULL);

	// IP �Է��� ���� Edit ��Ʈ��
	hEditIP = CreateWindow(L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		299, 311, 140, 35, // x, y, width, height
		hWnd, (HMENU)101, hInst, NULL);
	///////////////


}

void CScene_Start::Exit()
{
	DeleteAll();

	// Edit ��Ʈ���� �����Ѵ�.
	DestroyWindow(hEditNickname);
	DestroyWindow(hEditIP);

	// �Ҹ� ����
	CResourceMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\NewJeans.wav");
	CSound* pSound = CResourceMgr::GetInst()->FindSound(L"BGM_01");
	/*pSound->Stop();*/
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);

	HINSTANCE hInst{};
	HWND hWnd = CCore::GetInst()->GetMainHwnd();
	
	// EditBox �׸���
	EditBoxRender(_dc);
}

void CScene_Start::EditBoxRender(HDC _dc)
{
	Rectangle(_dc, 299, 271, 299 + 140, 271 + 22);
	Rectangle(_dc, 299, 311, 299 + 140, 311 + 22);

	// Edit ��Ʈ���� �ؽ�Ʈ�� �����´�.
	int iLenNickname = GetWindowTextLength(hEditNickname);
	int iLenIP = GetWindowTextLength(hEditIP);

	// Edit ��Ʈ���� �ؽ�Ʈ�� ������ ���۸� �Ҵ��Ѵ�.
	TCHAR* pStrNickname = new TCHAR[iLenNickname + 1];
	TCHAR* pStrIP = new TCHAR[iLenIP + 1];

	// Edit ��Ʈ���� �ؽ�Ʈ�� ���ۿ� �����Ѵ�.
	GetWindowText(hEditNickname, pStrNickname, iLenNickname + 1);
	GetWindowText(hEditIP, pStrIP, iLenIP + 1);

	// Edit ��Ʈ���� �ؽ�Ʈ�� ����Ѵ�.
	TextOut(_dc, 300, 273, pStrNickname, iLenNickname);
	TextOut(_dc, 300, 313, pStrIP, iLenIP);

	// ���۸� �����Ѵ�.
	delete[] pStrNickname;
	delete[] pStrIP;
}

void CScene_Start::update()
{
	CScene::update();

	if (KEY_TAP(KEY::LBUTTON)) {
		
		// ���콺 ��ǥ�� �޾ƿ´�.
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

		// ���콺�� StartButton ���� �ִ��� Ȯ���Ѵ�.
		CUI* pStartButton = (CUI*)FindObject(L"StartButton");
		CUI* pExitButton = (CUI*)FindObject(L"ExitButton");

		if (pStartButton->GetCollider()->PtInCollider(vMousePos)) {
			setId(hEditNickname, hEditIP);
			ChangeScene(SCENE_TYPE::SELECT);
		}

		// ���콺�� ExitButton ���� �ִ��� Ȯ���Ѵ�.
		if (pExitButton->GetCollider()->PtInCollider(vMousePos)) {
			// ���α׷� ����
			PostQuitMessage(0);
		}
	}
}