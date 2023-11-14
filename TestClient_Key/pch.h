// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS

// 여기에 미리 컴파일하려는 헤더 추가
#include <iostream>
#include <vector>
#include <array>
#include "framework.h"

using namespace std;

enum class KEY_STATE
{
	NONE,	// 눌리지 않음 (이전에 안눌림)
	TAP,	// 막 누른 시점 (이전에 안눌림)
	HOLD,	// 누르고 있는 시점 (이전에 눌림)
	AWAY,	// 막 땐 시점 (이전에 눌림)
};

// 사용할 키 값들
enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	ESC,
	TAB,
	LSHIFT,
	RSHIFT,
	LCONTROL,
	RCONTROL,
	LALT,
	RALT,
	ENTER,
	INSERT,
	HOME,
	PAGEUP,
	PAGEDOWN,
	END,
	DEL,
	SPACE,
	LBOTTON,

	LAST,
};

#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key)	KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) 	KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key)	KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key)	KEY_CHECK(key, KEY_STATE::NONE)

#define SINGLE(type) public: static type* GetInst() { static type mgr; return &mgr; }\
					 private: type(); ~type();

#endif //PCH_H
