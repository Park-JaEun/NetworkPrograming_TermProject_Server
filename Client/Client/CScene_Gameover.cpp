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

	// Gameover 배경
	CObject* pGameoverObj = new CBackground;
	pGameoverObj->SetName(L"Gameover");
	pGameoverObj->SetPos(Vec2(0.f, 0.f));
	pGameoverObj->SetScale(Vec2(vResolution.x, vResolution.y));

	CreateObject(pGameoverObj, GROUP_TYPE::BACKGROUND);
	///////////////////////


	// Robby 버튼 UI
	CUI* pRobbyButtonUI = new CUI;
	pRobbyButtonUI->SetName(L"RobbyButton");
	pRobbyButtonUI->SetPos(Vec2(0.f, 300.f));
	pRobbyButtonUI->SetScale(Vec2(100.f, 100.f));
	pRobbyButtonUI->CreateAnimator();
	pRobbyButtonUI->CreateCollider();
	pRobbyButtonUI->GetCollider()->SetScale(Vec2(100.f, 100.f));
	AddObject(pRobbyButtonUI, GROUP_TYPE::UI);

	// Quit 버튼 UI
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