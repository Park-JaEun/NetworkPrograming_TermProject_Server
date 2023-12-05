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
#include "CCollider.h"
#include "CPlayer.h"

CCore::CCore() :
	m_hWnd(nullptr), m_ptResolution{}, m_hDC(nullptr),
	m_hBit(nullptr), m_memDC(nullptr), m_arrBrush{}, m_arrPen{}, 
	m_sock{ INVALID_SOCKET }, m_bIsStart{ false }, m_bIsGameClear{ false }, m_bIsGameOver{ false }
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

	// 윈도우를 크기를 변경할 수 없고, 최대화 버튼을 없애도록 만든다.
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_SIZEBOX);
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

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

void CCore::Comunication()
{
	char buf[BUFSIZE];
	int retval;
	int size;
	bool bAllKeyNone = true;
	SOCKET sock = m_sock;

	if (!m_bIsGameClear && !m_bIsGameOver) {
		// send
		sendKeyBoardInput(sock);    // 키 입력 정보 전송

		// recv
		recvPlayerInfo(sock);       // 플레이어 정보 받기
		recvMonsterInfo(sock);      // 몬스터 정보 받기
		recvBossInfo(sock);         // 보스 정보 받기
		recvRabbitItemInfo(sock);   // 토끼 아이템 정보 받기
		recvCookieInfo(sock);		// 쿠키 정보 받기
		recvPlayerBulletInfo(sock);	// 플레이어 투사체 정보 받기
		recvBossBulletInfo(sock);	// 몬스터 투사체 정보 받기
		recvBossBulletInfo(sock);	// 보스 투사체 정보 받기
		recvGameStateSignal(sock);	// 게임 상태 패킷 받기
	}
}

void CCore::progress()
{
	// 소켓이 연결되어 있으면 통신
	if (m_sock != INVALID_SOCKET && m_bIsStart) {
		Comunication();
	}

	// Managers Update
	CTimer::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();
	CSceneMgr::GetInst()->update();

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