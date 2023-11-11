#include "pch.h"
#include "CKeyMgr.h"
#include "CCore.h"

// 내가 만든 키 값들을 윈도우 가상 키코드값으로 바꾸기
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
	// 윈도우 포커싱 알아내기
	HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	// 윈도우 포커싱
	if (hWnd != nullptr){ 
		for (int i = 0; i < (int)KEY::LAST; ++i){
			// 키가 눌려있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) {
				if (m_vecKey[i].bPrevPush) {
					// 이전에 눌려있었다면 KEY_STATE::HOLD
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else {
					// 눌려있지 않았다면 KEY_STATE::TAP
					m_vecKey[i].eState = KEY_STATE::TAP;
				}

				// 상태 갱신
				m_vecKey[i].bPrevPush = true;
			}
			// 키가 안눌려있다.
			else {
				if (m_vecKey[i].bPrevPush) {
					// 이전에 눌려있었다면 KEY_STATE::AWAY
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else {
					// 눌려있지 않았다면 KEY_STATE::NONE
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				// 상태 갱신
				m_vecKey[i].bPrevPush = false;
			}
		}
	}
	// 노 포커싱
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
