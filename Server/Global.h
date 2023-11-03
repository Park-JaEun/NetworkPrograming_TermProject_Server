#pragma once

#define MAX_PLAYER 3
#define BUFSIZE 512 
enum class CHARACTER_TYPE
{
	MINJI,
	HANNIE,
	DANIELLE,
	HEARIN,
	HYEIN,
};

enum class KEY_STATE
{
	NONE,   // ������ ���� (������ �ȴ���)
	TAP,   // �� ���� ���� (������ �ȴ���)
	HOLD,   // ������ �ִ� ���� (������ ����)
	AWAY,   // �� �� ���� (������ ����)
};

// ����� Ű ����
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

	LAST,
};

class Citem {
	int a;
};

class CPlayer{
	int a;
};

class CBullet{
	int a;
};
class CBoss {
	int a;

};
class CMonster {

};