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

	SC_RANK_PACKET* rankPacket = reinterpret_cast<SC_RANK_PACKET*>(buf);

	// 스코어 정보 받기 
	retval = recv(CCore::GetInst()->GetSocket(), (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(CCore::GetInst()->GetSocket(), buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(CCore::GetInst()->GetSocket());
		WSACleanup();
		return;
	}

	for (int i = 0; i < MAX_PLAYER; ++i) {
		CUI* pCharacterUI = new CUI;

		switch (static_cast<CHARACTER_TYPE>(rankPacket->character[i]))
		{
		case CHARACTER_TYPE::MINJI: 
		{
			pCharacterUI->SetName(L"Minji Score");
			pCharacterUI->SetPos(Vec2(128.f + (128.f * i), 155.f));
			pCharacterUI->SetScale(Vec2(110.f, 110.f));

			pCharacterUI->CreateAnimator();
		}
			break;
		case CHARACTER_TYPE::HANNIE: 
		{
			pCharacterUI->SetName(L"Hannie Score");
			pCharacterUI->SetPos(Vec2(128.f + (128.f * i), 155.f));
			pCharacterUI->SetScale(Vec2(110.f, 110.f));

			pCharacterUI->CreateAnimator();
		}
			break;
		case CHARACTER_TYPE::DANIELLE:
		{
			pCharacterUI->SetName(L"Danielle Score");
			pCharacterUI->SetPos(Vec2(128.f + (128.f * i), 155.f));
			pCharacterUI->SetScale(Vec2(110.f, 110.f));

			pCharacterUI->CreateAnimator();
		}
			break;
		case CHARACTER_TYPE::HAERIN:
		{
			pCharacterUI->SetName(L"Haerin Score");
			pCharacterUI->SetPos(Vec2(128.f + (128.f * i), 155.f));
			pCharacterUI->SetScale(Vec2(110.f, 110.f));

			pCharacterUI->CreateAnimator();
		}
			break;
		case CHARACTER_TYPE::HYEIN:
		{
			pCharacterUI->SetName(L"Hyein Score");
			pCharacterUI->SetPos(Vec2(128.f + (128.f * i), 155.f));
			pCharacterUI->SetScale(Vec2(110.f, 110.f));

			pCharacterUI->CreateAnimator();
		}
			break;
		default:
			break;
		}

		m_iScore[i] = rankPacket->score[i];
		CreateObject(pCharacterUI, GROUP_TYPE::UI);
	}

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

	// Robby 버튼 UI
	CUI* pRobbyButtonUI = new CUI;
	pRobbyButtonUI->SetName(L"RobbyButton");
	pRobbyButtonUI->SetPos(Vec2(40.f, 270.f));
	pRobbyButtonUI->SetScale(Vec2(100.f, 100.f));
	pRobbyButtonUI->CreateAnimator();
	pRobbyButtonUI->CreateCollider();
	pRobbyButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pRobbyButtonUI, GROUP_TYPE::UI);

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
		CUI* pRobbyButton = (CUI*)FindObject(L"RobbyButton");
		CUI* pQuitButton = (CUI*)FindObject(L"QuitButton");

		if (pRobbyButton->GetCollider()->PtInCollider(vMousePos)) {
			// 캐릭터 선택 화면으로 이동
			ChangeScene(SCENE_TYPE::SELECT);

			// 로비 버튼 선택 패킷 전송
			CS_SELECT_LOBBY_PACKET selectLobbyPacket;
			selectLobbyPacket.type = static_cast<char>(CS_PACKET_TYPE::CS_SELECT_LOBBY);
			selectLobbyPacket.id = CCore::GetInst()->GetID();
			size = sizeof(CS_SELECT_LOBBY_PACKET);

			retval = send(CCore::GetInst()->GetSocket(), reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(CCore::GetInst()->GetSocket(), reinterpret_cast<char*>(&selectLobbyPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(CCore::GetInst()->GetSocket());
				WSACleanup();
				return;
			}

			std::cout << "로비 버튼 선택 패킷 송신" << std::endl;
		}

		// 마우스가 ExitButton 위에 있는지 확인한다.
		if (pQuitButton->GetCollider()->PtInCollider(vMousePos)) {
			// 종료 버튼 선택 패킷 전송
			CS_SELECT_EXIT_PACKET selectQuitPacket;
			selectQuitPacket.type = static_cast<char>(CS_PACKET_TYPE::CS_SELECT_EXIT);
			selectQuitPacket.id = CCore::GetInst()->GetID();
			size = sizeof(CS_SELECT_EXIT_PACKET);

			retval = send(CCore::GetInst()->GetSocket(), reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(CCore::GetInst()->GetSocket(), reinterpret_cast<char*>(&selectQuitPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(CCore::GetInst()->GetSocket());
				WSACleanup();
				return;
			}

			std::cout << "종료 버튼 선택 패킷 송신" << std::endl;

			// 프로그램 종료
			PostQuitMessage(0);
		}
	}
}