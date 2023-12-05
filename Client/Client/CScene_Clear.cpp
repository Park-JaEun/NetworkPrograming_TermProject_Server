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

	// 애니메이터 생성
	pClearAniObj->CreateAnimator();

	CreateObject(pClearAniObj, GROUP_TYPE::BACKGROUND);

	// 순위 칠판
	CObject* pRankingBoard = new CBackground;
	pRankingBoard->SetName(L"Ranking Board");
	pRankingBoard->SetScale(Vec2(vResolution.x, vResolution.y));

	// 애니메이터 생성
	pRankingBoard->CreateAnimator();

	CreateObject(pRankingBoard, GROUP_TYPE::BACKGROUND);
	///////////////////////

	// Score 정보 받기
	recvRankInfo(CCore::GetInst()->GetSocket());

	// 왕관 UI
	CUI* pCrownUI = new CUI;

	// 가장 점수가 높은 사람의 ID를 찾는다.
	int iMaxScore = 0;
	int iMaxScoreID = 0;

	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (m_iScore[i] > iMaxScore) {
			iMaxScore = m_iScore[i];
			iMaxScoreID = i;
		}
	}

	// 가장 점수가 높은 사람의 캐릭터 위에 왕관 UI 생성
	pCrownUI->SetName(L"Crown");
	pCrownUI->SetPos(Vec2(160.f + (128.f * iMaxScoreID), 105.f));
	CreateObject(pCrownUI, GROUP_TYPE::UI);

	// Lobby 버튼 UI
	CUI* pLobbyButtonUI = new CUI;
	pLobbyButtonUI->SetName(L"LobbyButton");
	pLobbyButtonUI->SetPos(Vec2(40.f, 270.f));
	pLobbyButtonUI->SetScale(Vec2(100.f, 100.f));
	pLobbyButtonUI->CreateAnimator();
	pLobbyButtonUI->CreateCollider();
	pLobbyButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pLobbyButtonUI, GROUP_TYPE::UI);

	// Quit 버튼 UI
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

	// 점수 출력 (가장 높은 점수는 빨간색으로)
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

		// 마우스 좌표를 받아온다.
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

		// 마우스가 StartButton 위에 있는지 확인한다.
		CUI* pLobbyButton = (CUI*)FindObject(L"LobbyButton");
		CUI* pQuitButton = (CUI*)FindObject(L"QuitButton");

		if (pLobbyButton->GetCollider()->PtInCollider(vMousePos)) {
			// 캐릭터 선택 화면으로 이동
			ChangeScene(SCENE_TYPE::SELECT);
			// 로비 버튼 선택 패킷을 서버로 보낸다.
			sendLobbySignal(CCore::GetInst()->GetSocket());

			std::cout << "로비 버튼 선택 패킷 송신" << std::endl;
		}

		// 마우스가 ExitButton 위에 있는지 확인한다.
		if (pQuitButton->GetCollider()->PtInCollider(vMousePos)) {
			// 종료 패킷을 서버로 보낸다.
			sendExitSignal(CCore::GetInst()->GetSocket());
			std::cout << "종료 버튼 선택 패킷 송신" << std::endl;

			// 프로그램 종료
			PostQuitMessage(0);
		}
	}
}