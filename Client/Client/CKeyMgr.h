#pragma once
// Key Manager
// 1. 프레임 동기화: 동일 프레임 내에서 같은 키에 대해, 동일한 이벤트를 가져간다.
// 2. 키 입력 이벤트를 구체적으로 처리 (눌렀나? 놨나? 이전에 눌렀나? 이전에 놨나? 등등)

#include "define.h"

struct tKeyInfo
{
	KEY_STATE	eState;		// 키의 상태값
	bool		bPrevPush;	// 이전 프레임에서 눌렸는지 여부
};

class CKeyMgr
{
	SINGLE(CKeyMgr)
private:
	std::vector<tKeyInfo> m_vecKey;

public:
	Vec2 GetMousePos();		// 마우스 좌표를 받아온다.

public:
	void init();
	void update();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
};

