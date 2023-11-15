#include "pch.h"
#include "CKeyMgr.h"

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
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	// ������ ��Ŀ��
	if (hWnd != nullptr) {
		for (int i = 0; i < (int)KEY::LAST; ++i) {
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
