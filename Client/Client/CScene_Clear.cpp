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

	SC_RANK_PACKET* rankPacket = reinterpret_cast<SC_RANK_PACKET*>(buf);

	// ���ھ� ���� �ޱ� 
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

	// Robby ��ư UI
	CUI* pRobbyButtonUI = new CUI;
	pRobbyButtonUI->SetName(L"RobbyButton");
	pRobbyButtonUI->SetPos(Vec2(40.f, 270.f));
	pRobbyButtonUI->SetScale(Vec2(100.f, 100.f));
	pRobbyButtonUI->CreateAnimator();
	pRobbyButtonUI->CreateCollider();
	pRobbyButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pRobbyButtonUI, GROUP_TYPE::UI);

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
		CUI* pRobbyButton = (CUI*)FindObject(L"RobbyButton");
		CUI* pQuitButton = (CUI*)FindObject(L"QuitButton");

		if (pRobbyButton->GetCollider()->PtInCollider(vMousePos)) {
			// ĳ���� ���� ȭ������ �̵�
			ChangeScene(SCENE_TYPE::SELECT);

			// �κ� ��ư ���� ��Ŷ ����
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

			std::cout << "�κ� ��ư ���� ��Ŷ �۽�" << std::endl;
		}

		// ���콺�� ExitButton ���� �ִ��� Ȯ���Ѵ�.
		if (pQuitButton->GetCollider()->PtInCollider(vMousePos)) {
			// ���� ��ư ���� ��Ŷ ����
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

			std::cout << "���� ��ư ���� ��Ŷ �۽�" << std::endl;

			// ���α׷� ����
			PostQuitMessage(0);
		}
	}
}