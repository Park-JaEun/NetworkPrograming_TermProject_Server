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

	// BGM 틀기
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

	// 닉네임 입력을 위한 Edit 컨트롤
	hEditNickname = CreateWindow(L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		299, 271, 140, 35, // x, y, width, height
		hWnd, (HMENU)100, hInst, NULL);

	// IP 입력을 위한 Edit 컨트롤
	hEditIP = CreateWindow(L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		299, 311, 140, 35, // x, y, width, height
		hWnd, (HMENU)101, hInst, NULL);
	///////////////


}

void CScene_Start::Exit()
{
	DeleteAll();

	// Edit 컨트롤을 삭제한다.
	DestroyWindow(hEditNickname);
	DestroyWindow(hEditIP);

	// 소리 멈춤
	CResourceMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\NewJeans.wav");
	CSound* pSound = CResourceMgr::GetInst()->FindSound(L"BGM_01");
	/*pSound->Stop();*/
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);

	HINSTANCE hInst{};
	HWND hWnd = CCore::GetInst()->GetMainHwnd();
	
	// EditBox 그리기
	EditBoxRender(_dc);
}

void CScene_Start::EditBoxRender(HDC _dc)
{
	Rectangle(_dc, 299, 271, 299 + 140, 271 + 22);
	Rectangle(_dc, 299, 311, 299 + 140, 311 + 22);

	// Edit 컨트롤의 텍스트를 가져온다.
	int iLenNickname = GetWindowTextLength(hEditNickname);
	int iLenIP = GetWindowTextLength(hEditIP);

	// Edit 컨트롤의 텍스트를 저장할 버퍼를 할당한다.
	TCHAR* pStrNickname = new TCHAR[iLenNickname + 1];
	TCHAR* pStrIP = new TCHAR[iLenIP + 1];

	// Edit 컨트롤의 텍스트를 버퍼에 저장한다.
	GetWindowText(hEditNickname, pStrNickname, iLenNickname + 1);
	GetWindowText(hEditIP, pStrIP, iLenIP + 1);

	// Edit 컨트롤의 텍스트를 출력한다.
	TextOut(_dc, 300, 273, pStrNickname, iLenNickname);
	TextOut(_dc, 300, 313, pStrIP, iLenIP);

	// 버퍼를 해제한다.
	delete[] pStrNickname;
	delete[] pStrIP;
}

void CScene_Start::update()
{
	CScene::update();

	if (KEY_TAP(KEY::LBUTTON)) {
		
		// 마우스 좌표를 받아온다.
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

		// 마우스가 StartButton 위에 있는지 확인한다.
		CUI* pStartButton = (CUI*)FindObject(L"StartButton");
		CUI* pExitButton = (CUI*)FindObject(L"ExitButton");

		if (pStartButton->GetCollider()->PtInCollider(vMousePos)) {
			int iLenNickname = GetWindowTextLength(hEditNickname);
			int iLenIP = GetWindowTextLength(hEditIP);
			int iPlayerID{};	// 플레이어 ID
			int retval{};		// 리턴값
			int size{};			// 수신할 데이터 크기
			char buf[BUFSIZE];	// 수신 버퍼

			TCHAR* pStrNickname = new TCHAR[iLenNickname + 1];
			TCHAR* pStrIP = new TCHAR[iLenIP + 1];

			// 서버의 IP와 나의 닉네임을 가져온다.
			GetWindowText(hEditNickname, pStrNickname, iLenNickname + 1);
			GetWindowText(hEditIP, pStrIP, iLenIP + 1);

			// TCHAR -> char
			char* pStrIPChar = new char[iLenIP + 1];
			char* pStrNicknameChar = new char[iLenNickname + 1];

			for (int i = 0; i < iLenIP + 1; ++i) {
				pStrIPChar[i] = pStrIP[i];
			}

			for (int i = 0; i < iLenNickname + 1; ++i) {
				pStrNicknameChar[i] = pStrNickname[i];
			}

			// 서버에 접속
			SOCKET sock = CCore::GetInst()->GetSocket();

			// 소켓 생성 및 할당
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET) 
				err_quit("socket()");
			CCore::GetInst()->SetSocket(sock);

			// 서버 주소 설정
			struct sockaddr_in serveraddr;
			memset(&serveraddr, 0, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			inet_pton(AF_INET, pStrIPChar, &serveraddr.sin_addr);
			serveraddr.sin_port = htons(SERVERPORT);
			if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
				err_quit("connect()");

			// 닉네임 송신
			retval = send(sock, reinterpret_cast<char*>(&iLenNickname), sizeof(int), 0);
			retval = send(sock, (char*)pStrNicknameChar, iLenNickname, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				closesocket(sock);
				WSACleanup();
				return;
			}
			std::cout << "닉네임 송신" << std::endl;

			// 플레이어 ID 수신
			retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
			retval = recv(sock, buf, size, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				closesocket(sock);
				WSACleanup();
				return;
			}

			SC_MAKE_ID_PACKET* IDPacket = reinterpret_cast<SC_MAKE_ID_PACKET*>(buf);
			std::cout << "Player" << IDPacket->id <<" 연결 성공" << std::endl;

			// 플레이어 ID 저장
			CCore::GetInst()->SetID(IDPacket->id);

			// 플레이어 닉네임 저장
			CCore::GetInst()->SetNickName(pStrNickname);

			delete[] pStrNickname;
			delete[] pStrIP;
			delete[] pStrIPChar;
			delete[] pStrNicknameChar;

			ChangeScene(SCENE_TYPE::SELECT);
		}

		// 마우스가 ExitButton 위에 있는지 확인한다.
		if (pExitButton->GetCollider()->PtInCollider(vMousePos)) {
			// 프로그램 종료
			PostQuitMessage(0);
		}
	}
}