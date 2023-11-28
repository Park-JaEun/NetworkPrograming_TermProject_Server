#include "pch.h"
#include "CKeyMgr.h"
#include "CCore.h"

// ���� ���� Ű ������ ������ ���� Ű�ڵ尪���� �ٲٱ�
std::array<int, (int)KEY::LAST> g_arrVK =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_SPACE,

	VK_LBUTTON,

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,

};

CKeyMgr::CKeyMgr()
{}

CKeyMgr::~CKeyMgr()
{}

Vec2 CKeyMgr::GetMousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &pt);

	return Vec2((float)pt.x, (float)pt.y);
}

void CKeyMgr::init()
{
	m_vecKey.reserve((int)KEY::LAST);

	for (int i = 0; i < (int)KEY::LAST; ++i)
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
}

void CKeyMgr::update()
{
	// ������ ��Ŀ�� �˾Ƴ���
	HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	// ������ ��Ŀ��
	if (hWnd != nullptr){ 
		for (int i = 0; i < (int)KEY::LAST; ++i){
			// g_arrVK[i]�� ���ȴ�.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) {
				if (m_vecKey[i].bPrevPush) {
					// ������ �����־��ٸ� KEY_STATE::HOLD
					m_vecKey[i].eState = KEY_STATE::HOLD;
					CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
				}
				else {
					// �������� �ʾҴٸ� KEY_STATE::TAP
					m_vecKey[i].eState = KEY_STATE::TAP;
					CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
				}

				// ���� ����
				m_vecKey[i].bPrevPush = true;
			}
			// Ű�� �ȴ����ִ�.
			else {
				if (m_vecKey[i].bPrevPush) {
					// ������ �����־��ٸ� KEY_STATE::AWAY
					m_vecKey[i].eState = KEY_STATE::AWAY;
					CCore::GetInst()->SetKey(i, KEY_STATE::AWAY);
				}
				else {
					// �������� �ʾҴٸ� KEY_STATE::NONE
					m_vecKey[i].eState = KEY_STATE::NONE;
					CCore::GetInst()->SetKey(i, KEY_STATE::NONE);
				}

				// ���� ����
				m_vecKey[i].bPrevPush = false;
			}
		}
	}
	// �� ��Ŀ��
	else {
		for (int i = 0; i < (int)KEY::LAST; ++i) {
			m_vecKey[i].bPrevPush = false;
			
			if (m_vecKey[i].eState == KEY_STATE::TAP || m_vecKey[i].eState == KEY_STATE::HOLD) {
				m_vecKey[i].eState = KEY_STATE::AWAY;
				CCore::GetInst()->SetKey(i, KEY_STATE::AWAY);

			}
			else if (m_vecKey[i].eState == KEY_STATE::AWAY) {
				m_vecKey[i].eState = KEY_STATE::NONE;
				CCore::GetInst()->SetKey(i, KEY_STATE::NONE);

			}
		}
	}

	
}
