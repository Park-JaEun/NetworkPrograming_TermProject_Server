#include "pch.h"
#include "CKeyMgr.h"

CS_KEYBOARD_INPUT_PACKET inputkey[(int)KEY::LAST] = {};

// 내가 만든 키 값들을 윈도우 가상 키코드값으로 바꾸기
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
	HWND hWnd = GetFocus();

	// 윈도우 포커싱
	if (hWnd != nullptr) {
		for (int i = 0; i < (int)KEY::LAST; ++i) {
			// 키가 눌려있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) {
				if (m_vecKey[i].bPrevPush) {
					// 이전에 눌려있었다면 KEY_STATE::HOLD
					m_vecKey[i].eState = KEY_STATE::HOLD;

				}
				else {
					// 눌려있지 않았다면 KEY_STATE::TAP
					m_vecKey[i].eState = KEY_STATE::TAP;

					switch (g_arrVK[i])
					{
					case VK_LEFT:
						inputkey[i].key = KEY::LEFT;
						break;
					case VK_RIGHT:
						inputkey[i].key = KEY::RIGHT;
						break;
					case VK_UP:
						inputkey[i].key = KEY::UP;
						break;
					case VK_DOWN:
						inputkey[i].key = KEY::DOWN;
						break;
					case VK_SPACE:
						inputkey[i].key = KEY::SPACE;
						break;
					case VK_LBUTTON:
						inputkey[i].key = KEY::LBUTTON;
						break;
					case VK_NUMPAD0:
						inputkey[i].key = KEY::NUMPAD0;
						break;
					case VK_NUMPAD1:
						inputkey[i].key = KEY::NUMPAD1;
						break;
					case VK_NUMPAD2:
						inputkey[i].key = KEY::NUMPAD2;
						break;
					case VK_NUMPAD3:
						inputkey[i].key = KEY::NUMPAD3;
						break;
					case VK_NUMPAD4:
						inputkey[i].key = KEY::NUMPAD4;
						break;
					case VK_NUMPAD5:
						inputkey[i].key = KEY::NUMPAD5;
						break;
					case VK_NUMPAD6:
						inputkey[i].key = KEY::NUMPAD6;
						break;
					case VK_NUMPAD7:
						inputkey[i].key = KEY::NUMPAD7;
						break;
					case VK_NUMPAD8:
						inputkey[i].key = KEY::NUMPAD8;
						break;
					case VK_NUMPAD9:
						inputkey[i].key = KEY::NUMPAD9;
						break;
					}

					inputkey[i].key_state = KEY_STATE::TAP;
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
				inputkey[i].key_state = KEY_STATE::NONE;
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
