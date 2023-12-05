#include "pch.h"
#include "CScene_Gameover.h"
#include "CCore.h"
#include "CBackground.h"
#include "CObject.h"
#include "CUI.h"
#include "CCollider.h"
#include "CKeyMgr.h"

CScene_Gameover::CScene_Gameover()
{
}

CScene_Gameover::~CScene_Gameover()
{
}

void CScene_Gameover::update()
{
	CScene::update();
}

void CScene_Gameover::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	char buf[BUFSIZE]{};
	int retval;
	int size;

	///////////////////////
	// Background Object //
	///////////////////////

	// Gameover ���
	CObject* pGameoverObj = new CBackground;
	pGameoverObj->SetName(L"Gameover");
	pGameoverObj->SetPos(Vec2(0.f, 0.f));
	pGameoverObj->SetScale(Vec2(vResolution.x, vResolution.y));

	CreateObject(pGameoverObj, GROUP_TYPE::BACKGROUND);
	///////////////////////


	// Robby ��ư UI
	CUI* pLobbyButtonUI = new CUI;
	pLobbyButtonUI->SetName(L"LobbyButton");
	pLobbyButtonUI->SetPos(Vec2(0.f, 300.f));
	pLobbyButtonUI->SetScale(Vec2(100.f, 100.f));
	pLobbyButtonUI->CreateAnimator();
	pLobbyButtonUI->CreateCollider();
	pLobbyButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pLobbyButtonUI, GROUP_TYPE::UI);

	// Quit ��ư UI
	CUI* pQuitButtonUI = new CUI;
	pQuitButtonUI->SetName(L"QuitButton");
	pQuitButtonUI->SetPos(Vec2(540.f, 300.f));
	pQuitButtonUI->SetScale(Vec2(100.f, 100.f));
	pQuitButtonUI->CreateAnimator();
	pQuitButtonUI->CreateCollider();
	pQuitButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pQuitButtonUI, GROUP_TYPE::UI);
}

void CScene_Gameover::Exit()
{
	DeleteAll();
}

void CScene_Gameover::render(HDC _dc)
{
	CScene::render(_dc);

	int size;
	int retval;

	if (KEY_TAP(KEY::LBUTTON)) {

		// ���콺 ��ǥ�� �޾ƿ´�.
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

		// ���콺�� StartButton ���� �ִ��� Ȯ���Ѵ�.
		CUI* pLobbyButton = (CUI*)FindObject(L"LobbyButton");
		CUI* pQuitButton = (CUI*)FindObject(L"QuitButton");

		if (pLobbyButton->GetCollider()->PtInCollider(vMousePos)) {
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