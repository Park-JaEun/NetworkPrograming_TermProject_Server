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

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,

	VK_ESCAPE,
	VK_TAB,
	VK_LSHIFT,
	VK_RSHIFT,
	VK_LCONTROL,
	VK_RCONTROL,
	VK_LMENU,
	VK_RMENU,
	VK_RETURN,
	VK_INSERT,
	VK_HOME,
	VK_PRIOR,
	VK_NEXT,
	VK_END,
	VK_DELETE,
	VK_SPACE,
	VK_LBUTTON,
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

	m_vecKey[(int)KEY::LEFT].eState;
}

void CKeyMgr::update()
{
	// ������ ��Ŀ�� �˾Ƴ���
	HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	// ������ ��Ŀ��
	if (hWnd != nullptr){ 
		for (int i = 0; i < (int)KEY::LAST; ++i){
			// Ű�� �����ִ�.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) {
				if (m_vecKey[i].bPrevPush) {
					// ������ �����־��ٸ� KEY_STATE::HOLD
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else {
					// �������� �ʾҴٸ� KEY_STATE::TAP
					m_vecKey[i].eState = KEY_STATE::TAP;
				}

				// ���� ����
				m_vecKey[i].bPrevPush = true;
			}
			// Ű�� �ȴ����ִ�.
			else {
				if (m_vecKey[i].bPrevPush) {
					// ������ �����־��ٸ� KEY_STATE::AWAY
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else {
					// �������� �ʾҴٸ� KEY_STATE::NONE
					m_vecKey[i].eState = KEY_STATE::NONE;
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
			}
			else if (m_vecKey[i].eState == KEY_STATE::AWAY) {
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}

	
}
