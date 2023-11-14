#pragma once

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

// Scene에 존재하는 Object들을 분류하기 위한 enum class
// 표시되어있는 순서대로 Object가 그려진다.
enum class GROUP_TYPE
{
	DEFAULT,
	BACKGROUND,
	FLOOR,
	MONSTER,
	BOSS,
	PLAYER,
	BULLET_PLAYER,
	BULLET_MONSTER,
	BULLET_BOSS,
	MISSILE_BOSS,
	UI,


	END = 32,
};

enum class SCENE_TYPE
{
	START,
	MAIN,

	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,

	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,


	END,
};

enum class PLAYER_STATE
{
	IDLE,
	RUN,
	HIT,
	DIE,
};

enum class MONSTER_STATE
{
	IDLE,
	MOVE,
	HIT,
	ATTACK,
	DIE,
};

enum class BOSS_STATE
{
	IDLE,
	ATTACK,
	DIE,
};

enum class CHARACTER_TYPE
{
	MINJI,
	HANNIE,
	DANIELLE,
	HEARIN,
	HYEIN,
};