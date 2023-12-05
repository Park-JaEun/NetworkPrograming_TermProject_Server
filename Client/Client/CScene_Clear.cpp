#include "pch.h"
#include "CScene_Clear.h"
#include "CCore.h"
#include "CBackground.h"
#include "CObject.h"
#include "CUI.h"
#include "CCollider.h"
#include "CKeyMgr.h"

CScene_Clear::CScene_Clear() : m_bClearAniEnd{ false }, m_iScore{}
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
	char buf[BUFSIZE]{};
	int retval;
	int size;

	///////////////////////
	// Background Object //
	///////////////////////
	CObject* pClearAniObj = new CBackground;
	pClearAniObj->SetName(L"Clear Animation");
	pClearAniObj->SetPos(Vec2(0.f, 0.f));
	pClearAniObj->SetScale(Vec2(vResolution.x, vResolution.y));

	// �ִϸ����� ����
	pClearAniObj->CreateAnimator();

	CreateObject(pClearAniObj, GROUP_TYPE::BACKGROUND);

	// ���� ĥ��
	CObject* pRankingBoard = new CBackground;
	pRankingBoard->SetName(L"Ranking Board");
	pRankingBoard->SetScale(Vec2(vResolution.x, vResolution.y));

	// �ִϸ����� ����
	pRankingBoard->CreateAnimator();

	CreateObject(pRankingBoard, GROUP_TYPE::BACKGROUND);
	///////////////////////

	// Score ���� �ޱ�
	recvRankInfo(CCore::GetInst()->GetSocket());

	// �հ� UI
	CUI* pCrownUI = new CUI;

	// ���� ������ ���� ����� ID�� ã�´�.
	int iMaxScore = 0;
	int iMaxScoreID = 0;

	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (m_iScore[i] > iMaxScore) {
			iMaxScore = m_iScore[i];
			iMaxScoreID = i;
		}
	}

	// ���� ������ ���� ����� ĳ���� ���� �հ� UI ����
	pCrownUI->SetName(L"Crown");
	pCrownUI->SetPos(Vec2(160.f + (128.f * iMaxScoreID), 105.f));
	CreateObject(pCrownUI, GROUP_TYPE::UI);

	// Lobby ��ư UI
	CUI* pLobbyButtonUI = new CUI;
	pLobbyButtonUI->SetName(L"LobbyButton");
	pLobbyButtonUI->SetPos(Vec2(40.f, 270.f));
	pLobbyButtonUI->SetScale(Vec2(100.f, 100.f));
	pLobbyButtonUI->CreateAnimator();
	pLobbyButtonUI->CreateCollider();
	pLobbyButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pLobbyButtonUI, GROUP_TYPE::UI);

	// Quit ��ư UI
	CUI* pQuitButtonUI = new CUI;
	pQuitButtonUI->SetName(L"QuitButton");
	pQuitButtonUI->SetPos(Vec2(500.f, 270.f));
	pQuitButtonUI->SetScale(Vec2(100.f, 100.f));
	pQuitButtonUI->CreateAnimator();
	pQuitButtonUI->CreateCollider();
	pQuitButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pQuitButtonUI, GROUP_TYPE::UI);
}

void CScene_Clear::Exit()
{
	DeleteAll();
}

void CScene_Clear::render(HDC _dc)
{
	CScene::render(_dc);

	int size;
	int retval;

	// ���� ��� (���� ���� ������ ����������)
	if (m_bClearAniEnd) {
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (m_iScore[i] == *std::max_element(m_iScore, m_iScore + MAX_PLAYER))
				SetTextColor(_dc, RGB(255, 0, 0));
			else
				SetTextColor(_dc, RGB(0, 255, 0));

			TextOut(_dc, 170 + (128 * i), 270, std::to_wstring(m_iScore[i]).c_str(), std::to_wstring(m_iScore[i]).size());
		}
	}
	
	if (KEY_TAP(KEY::LBUTTON)) {

		// ���콺 ��ǥ�� �޾ƿ´�.
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

		// ���콺�� StartButton ���� �ִ��� Ȯ���Ѵ�.
		CUI* pLobbyButton = (CUI*)FindObject(L"LobbyButton");
		CUI* pQuitButton = (CUI*)FindObject(L"QuitButton");

		if (pLobbyButton->GetCollider()->PtInCollider(vMousePos)) {
			// ĳ���� ���� ȭ������ �̵�
			ChangeScene(SCENE_TYPE::SELECT);
			// �κ� ��ư ���� ��Ŷ�� ������ ������.
			sendLobbySignal(CCore::GetInst()->GetSocket());

			std::cout << "�κ� ��ư ���� ��Ŷ �۽�" << std::endl;
		}

		// ���콺�� ExitButton ���� �ִ��� Ȯ���Ѵ�.
		if (pQuitButton->GetCollider()->PtInCollider(vMousePos)) {
			// ���� ��Ŷ�� ������ ������.
			sendExitSignal(CCore::GetInst()->GetSocket());
			std::cout << "���� ��ư ���� ��Ŷ �۽�" << std::endl;

			// ���α׷� ����
			PostQuitMessage(0);
		}
	}
}