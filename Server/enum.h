#pragma once

enum class KEY_STATE
{
	NONE,	// ������ ���� (������ �ȴ���)
	TAP,	// �� ���� ���� (������ �ȴ���)
	HOLD,	// ������ �ִ� ���� (������ ����)
	AWAY,	// �� �� ���� (������ ����)
};

// ����� Ű ����
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

// Scene�� �����ϴ� Object���� �з��ϱ� ���� enum class
// ǥ�õǾ��ִ� ������� Object�� �׷�����.
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