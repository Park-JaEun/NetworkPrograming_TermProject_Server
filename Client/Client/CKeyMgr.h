#pragma once
// Key Manager
// 1. ������ ����ȭ: ���� ������ ������ ���� Ű�� ����, ������ �̺�Ʈ�� ��������.
// 2. Ű �Է� �̺�Ʈ�� ��ü������ ó�� (������? ����? ������ ������? ������ ����? ���)

#include "define.h"

struct tKeyInfo
{
	KEY_STATE	eState;		// Ű�� ���°�
	bool		bPrevPush;	// ���� �����ӿ��� ���ȴ��� ����
};

class CKeyMgr
{
	SINGLE(CKeyMgr)
private:
	std::vector<tKeyInfo> m_vecKey;

public:
	Vec2 GetMousePos();		// ���콺 ��ǥ�� �޾ƿ´�.

public:
	void init();
	void update();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
};

