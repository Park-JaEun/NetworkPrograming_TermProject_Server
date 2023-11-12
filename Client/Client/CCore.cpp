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

CCore::CCore() : m_hWnd(nullptr), m_ptResolution{}, m_hDC(nullptr), m_hBit(nullptr), m_memDC(nullptr), m_arrBrush{}, m_arrPen{}
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	// ��� �귯�� ����
	for (HPEN& _hPen : m_arrPen)
		DeleteObject(_hPen);

	for (HBRUSH& _hBrush : m_arrBrush)
		DeleteObject(_hBrush);
}


int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// �ػ󵵿� �°� ������ ũ�� ����
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	// �����쿡 �׸��� �׷��� DC�� �����
	m_hDC = GetDC(m_hWnd);

	// ���� ���۸� �뵵�� ��Ʈ�ʰ� DC�� �����.
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	// ���� ����� �� �� �귯�� ����
	CreateBrushPen();

	// Init Managers
	CPathMgr::GetInst()->init();
	CTimer::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();

	return S_OK;
}

void CCore::CreateBrushPen()
{
	// hollow brush (���� �귯��)
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// red pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	// green pen
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	// blue pen
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

void CCore::progress()
{
	// Managers Update
	CTimer::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();

	CSceneMgr::GetInst()->update();
	CCollisionMgr::GetInst()->update();

	///////////////
	// Rendering //
	///////////////
	
	// ȭ�� Clear
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	// �׸���
	CSceneMgr::GetInst()->render(m_memDC);

	// �ĸ������ ������ ������� �ű��.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY/* �״�� ���� */);

	// Ÿ��Ʋ �ٿ� ������ render
	CTimer::GetInst()->render();

	// �̺�Ʈ�� �� �������� ������Ʈ (����ó��)
	CEventMgr::GetInst()->update();
}