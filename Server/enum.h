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

	SPACE,

	LBUTTON,

	NUMPAD0,
	NUMPAD1,
	NUMPAD2,
	NUMPAD3,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD7,
	NUMPAD8,
	NUMPAD9,

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
	ITEM,
	BULLET_PLAYER,
	BULLET_MONSTER,
	BULLET_BOSS,
	MISSILE_BOSS,
	UI,


	END = 32,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,

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