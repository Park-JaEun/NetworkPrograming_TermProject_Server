#include "pch.h"
#include "CCore.h"
#include "CObject.h"

#include "CTimer.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CScene.h"
#include "CSound.h"


CCore::CCore() :
	m_hWnd(nullptr), m_ptResolution{}, m_hDC(nullptr),
	m_hBit(nullptr), m_memDC(nullptr), m_arrBrush{}, m_arrPen{}, 
	m_sock{ INVALID_SOCKET }, m_bIsStart{ false }
{
	// 키 정보 초기화
	for (int i = 0; i < (int)KEY::LAST; ++i) {
		m_inputkey[i].inputs->key = KEY(i);
		m_inputkey[i].inputs->key_state = KEY_STATE::NONE;
	}
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	// 펜과 브러쉬 제거
	for (HPEN& _hPen : m_arrPen)
		DeleteObject(_hPen);

	for (HBRUSH& _hBrush : m_arrBrush)
		DeleteObject(_hBrush);
}


int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 크기 조정
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	// 윈도우에 그림을 그려줄 DC를 만들기
	m_hDC = GetDC(m_hWnd);

	// 더블 버퍼링 용도의 비트맵과 DC를 만든다.
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	// 자주 사용할 펜 및 브러쉬 생성
	CreateBrushPen();

	// Init Managers
	CPathMgr::GetInst()->init();
	CTimer::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CSoundMgr::GetInst()->init();

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return S_FALSE;

	return S_OK;
}

void CCore::CreateBrushPen()
{
	// hollow brush (투명 브러쉬)
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// red pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	// green pen
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	// blue pen
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

void CCore::CommunicationToServer()
{
	////////////
	// send() //
	////////////
	int size;
	int retval;
	char buf[BUFSIZE];

	SOCKET sock = m_sock;
	//CS_PACKET_TYPE type = CS_PACKET_TYPE(rand() % 5 + 1); // send()타입 선택 

	//switch (type) 
	//{
	////case CS_PACKET_TYPE::CS_KEYBOARD_INPUT: 
	////{
	////	CS_KEYBOARD_INPUT_PACKET p;
	////	p.type = static_cast<char>(CS_PACKET_TYPE::CS_KEYBOARD_INPUT);
	////	size = sizeof(p);
	////	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	////	retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
	////}
	////break;

	///*case CS_PACKET_TYPE::CS_SELECT_LOBBY: 
	//{
	//	CS_SELECT_LOBBY_PACKET p;
	//	p.type = static_cast<char>(CS_PACKET_TYPE::CS_SELECT_LOBBY);
	//	size = sizeof(p);
	//	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//	retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
	//}
	//break;

	//case CS_PACKET_TYPE::CS_SELECT_EXIT: 
	//{
	//	CS_SELECT_EXIT_PACKET p;
	//	p.type = static_cast<char>(CS_PACKET_TYPE::CS_SELECT_EXIT);
	//	size = sizeof(p);
	//	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	//	retval = send(sock, reinterpret_cast<char*>(&p), size, 0);
	//}
	//break;*/
	//default:
	//	break;
	//}

	// 오류 처리
	/*if (retval == SOCKET_ERROR) {
		err_display("send()");
		closesocket(sock);
		WSACleanup();
		return;
	}*/

	////////////
	// recv() //
	////////////
	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	switch (buf[0]) 
	{
		case int(SC_PACKET_TYPE::SELECT_CHARACTER): 
		{
			SELECT_CHARACTER_PACKET* p = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
		}
		break;

		case int(SC_PACKET_TYPE::SC_GAME_START): 
		{
			SC_GAME_START_PACKET* p = reinterpret_cast<SC_GAME_START_PACKET*>(buf);
		}
		break;

		case int(SC_PACKET_TYPE::SC_GAME_OVER): 
		{
			SC_GAME_OVER_PACKET* p = reinterpret_cast<SC_GAME_OVER_PACKET*>(buf);
		}
		break;

		case int(SC_PACKET_TYPE::SC_GAME_CLEAR): 
		{
			SC_GAME_CLEAR_PACKET* p = reinterpret_cast<SC_GAME_CLEAR_PACKET*>(buf);
		}
		break;

		case int(SC_PACKET_TYPE::SC_PLAYER): 
		{
			SC_PLAYER_PACKET* p = reinterpret_cast<SC_PLAYER_PACKET*>(buf);

			// 
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			CPlayer* pPlayer = (CPlayer*)pCurScene->FindObject(L"player");
			pPlayer->SetPos(p->playerPos);
			pPlayer->SetState(p->playerState);
			pPlayer->SetDir(p->playerDir);

			std::cout << "SC_PLAYER_PACKET 받음" << std::endl;
		}
		break;

		case int(SC_PACKET_TYPE::SC_MONSTER): 
		{
			SC_MONSTER_PACKET* p = reinterpret_cast<SC_MONSTER_PACKET*>(buf);
		}
		break;

		case int(SC_PACKET_TYPE::SC_BOSS): 
		{
			SC_BOSS_PACKET* p = reinterpret_cast<SC_BOSS_PACKET*>(buf);
		}
		break;

		case int(SC_PACKET_TYPE::SC_BULLET): 
		{
			SC_BULLET_PACKET* p = reinterpret_cast<SC_BULLET_PACKET*>(buf);
		}
		break;

		/*case int(SC_PACKET_TYPE::SC_ITEM): 
		{
			SC_ITEM_PACKET* p = reinterpret_cast<SC_ITEM_PACKET*>(buf);
		}
		break;*/

		case int(SC_PACKET_TYPE::SC_RANK): 
		{
			SC_RANK_PACKET* p = reinterpret_cast<SC_RANK_PACKET*>(buf);
		}
		break;

		default:
			break;
	}
}

void CCore::TestSendKeyInput()
{
	char buf[BUFSIZE];
	int retval;
	int size;
	bool bAllKeyNone = true;
	SOCKET sock = m_sock;

	CS_KEYBOARD_INPUT_PACKET cs_p;
	cs_p.type = static_cast<char>(CS_PACKET_TYPE::CS_KEYBOARD_INPUT);
	cs_p.keyCount = 0;
	size = sizeof(cs_p);

	for (int i = 0; i < (int)KEY::LAST; ++i) {
		if (m_inputkey[i].inputs->key_state != KEY_STATE::NONE) {
			if(bAllKeyNone)
				bAllKeyNone = false;

			if (cs_p.keyCount < MAX_KEYS) {
				cs_p.inputs[cs_p.keyCount].key = m_inputkey[i].inputs->key;
				cs_p.inputs[cs_p.keyCount].key_state = m_inputkey[i].inputs->key_state;
				cs_p.keyCount++;
			}
		}
	}

	if (cs_p.keyCount > 0 || bAllKeyNone) {
		if (bAllKeyNone) {
			cs_p.inputs[0].key = KEY::LAST;
			cs_p.inputs[0].key_state = KEY_STATE::NONE;
			cs_p.keyCount = 1;
			bAllKeyNone = true;
		}

		size = sizeof(cs_p);
		retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(sock, reinterpret_cast<char*>(&cs_p), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return;
		}
	}

	////////////
	// recv() //
	////////////
	
	// 플레이어들 정보 받기
	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	SC_PLAYER_PACKET* pPlayerPacket = reinterpret_cast<SC_PLAYER_PACKET*>(buf);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CPlayer* pPlayer = (CPlayer*)pCurScene->FindObject(L"Player" + std::to_wstring(pPlayerPacket->playerID));
	
	pPlayer->SetPos(pPlayerPacket->playerPos);
	pPlayer->SetState(pPlayerPacket->playerState);
	pPlayer->SetDir(pPlayerPacket->playerDir);

	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	pPlayer = (CPlayer*)pCurScene->FindObject(L"Player" + std::to_wstring(pPlayerPacket->playerID));
	pPlayer->SetPos(pPlayerPacket->playerPos);
	pPlayer->SetState(pPlayerPacket->playerState);
	pPlayer->SetDir(pPlayerPacket->playerDir);

	retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	pPlayer = (CPlayer*)pCurScene->FindObject(L"Player" + std::to_wstring(pPlayerPacket->playerID));
	pPlayer->SetPos(pPlayerPacket->playerPos);
	pPlayer->SetState(pPlayerPacket->playerState);
	pPlayer->SetDir(pPlayerPacket->playerDir);

	// 몬스터 정보 받기
	SC_MONSTER_PACKET* pMonsterPacket = reinterpret_cast<SC_MONSTER_PACKET*>(buf);

	const std::vector<CObject*>& vecMonster = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	for (CObject* pMonster : vecMonster) {
		retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
		retval = recv(sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		pMonster->SetPos(pMonsterPacket->monsterPos);
		((CMonster*)pMonster)->SetDir(pMonsterPacket->monsterDir);
		((CMonster*)pMonster)->SetState(pMonsterPacket->monsterState);
	}

	// 아이템 정보 받기
	SC_ITEM_PACKET* pItemPacket = reinterpret_cast<SC_ITEM_PACKET*>(buf);
	const std::vector<CObject*>& vecItem = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::ITEM);

	// 아이템 수 받기
	int itemCount = 0;
	retval = recv(sock, reinterpret_cast<char*>(&itemCount), sizeof(itemCount), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	// 정보 받기
	for (int i = 0; i < itemCount; ++i) {
		retval = recv(sock, (char*)&size, sizeof(int), MSG_WAITALL);
		retval = recv(sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		if (pItemPacket->itemIsDead) {
			// 삭제된 아이템은 클라이언트 내에서 삭제
			DeleteObject(vecItem[i]);
		}
		else {
			// 삭제되지 않은 아이템들은 업데이트
			vecItem[i]->SetPos(pItemPacket->itemPos);
		}
	}
}

void CCore::progress()
{
	// 소켓이 연결되어 있으면 통신
	if (m_sock != INVALID_SOCKET && m_bIsStart) {
		TestSendKeyInput();
		//CommunicationToServer();
	}

	// Managers Update
	CTimer::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();

	CSceneMgr::GetInst()->update();
	//CCollisionMgr::GetInst()->update();

	///////////////
	// Rendering //
	///////////////
	
	// 화면 Clear
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	// 그리기
	CSceneMgr::GetInst()->render(m_memDC);

	// 후면버퍼의 내용을 윈도우로 옮긴다.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY/* 그대로 복사 */);

	// 타이틀 바에 프레임 render
	CTimer::GetInst()->render();

	// 이벤트는 맨 마지막에 업데이트 (지연처리)
	CEventMgr::GetInst()->update();
}