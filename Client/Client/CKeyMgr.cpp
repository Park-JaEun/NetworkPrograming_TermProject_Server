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

					switch (g_arrVK[i])
					{
					case VK_LEFT:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_RIGHT:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_UP:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_DOWN:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_SPACE:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_LBUTTON:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD0:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD1:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD2:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD3:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD4:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD5:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD6:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD7:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD8:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					case VK_NUMPAD9:
						CCore::GetInst()->SetKey(i, KEY_STATE::HOLD);
						break;
					}
				}
				else {
					// �������� �ʾҴٸ� KEY_STATE::TAP
					m_vecKey[i].eState = KEY_STATE::TAP;

					switch (g_arrVK[i])
					{
					case VK_LEFT:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_RIGHT:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_UP:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_DOWN:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_SPACE:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_LBUTTON:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD0:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD1:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD2:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD3:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD4:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD5:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD6:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD7:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD8:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					case VK_NUMPAD9:
						CCore::GetInst()->SetKey(i, KEY_STATE::TAP);
						break;
					}

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
