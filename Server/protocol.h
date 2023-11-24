#pragma once
#include "define.h"
#include "struct.h"
#include "enum.h"
#include "function.h"
#include "CBoss.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CBullet.h"
//#include "CItem.h"

////////////////////////
// ���� �� Ŭ���̾�Ʈ //
////////////////////////

// ��Ŷ Ÿ�� ����
enum class SC_PACKET_TYPE {
	SELECT_CHARACTER = 1,
	SC_MAKE_ID,
	SC_INIT,
	SC_GAME_START,
	SC_GAME_OVER,
	SC_GAME_CLEAR,
	SC_PLAYER,
	SC_MONSTER,
	SC_BOSS,
	SC_BULLET,
	//SC_ITEM,
	SC_RANK,
};

#pragma pack(push, 1) // 1����Ʈ ũ��� ����
// ID �Ҵ� ���� ��Ŷ
struct SC_MAKE_ID_PACKET {
	char type;	// ��Ŷ Ÿ��
	int id;		// �Ҵ��� id(1p, 2p, 3p)
};

// �ʱ�ȭ ��ȣ ���� ���� ��Ŷ
struct SC_INIT_PACKET {
	char type; // ��Ŷ Ÿ��
};

// ���� ���� ��ȣ ���� ���� ��Ŷ
struct SC_GAME_START_PACKET {
	char type; // ��Ŷ Ÿ��
};

// ���� ���� ��ȣ ���� ���� ��Ŷ
struct SC_GAME_OVER_PACKET {
	char type; // ��Ŷ Ÿ��
};

// ���� Ŭ���� ��ȣ ���� ��Ŷ
struct SC_GAME_CLEAR_PACKET {
	char type; // ��Ŷ Ÿ��
};

// �÷��̾� ���� ���� ���� ��Ŷ
struct SC_PLAYER_PACKET {
	char type;			// ��Ŷ Ÿ��
	// �÷��̾� ����
	Vec2 playerPos;					// �÷��̾� ��ġ
	PLAYER_STATE playerState;		// �÷��̾� ����
	bool playerDir;					// �÷��̾� ����
};

// ���� ���� ���� ���� ��Ŷ
struct SC_MONSTER_PACKET {
	char type;			// ��Ŷ Ÿ��
	CMonster  monster;	// ���� ����
};

// ���� ���� ���� ���� ��Ŷ
struct SC_BOSS_PACKET {
	char type;		// ��Ŷ Ÿ��
	CBoss  boss;	// ���� ����
};

// ����ü ���� ���� ���� ��Ŷ
struct SC_BULLET_PACKET {
	char type;			// ��Ŷ Ÿ��
	CBullet  bullet;	// ����ü ����
};

// ������ ���� ���� ���� ��Ŷ
//struct SC_ITEM_PACKET {
//	char type;		// ��Ŷ Ÿ��
//	Citem  item;	// ����ü ����
//};

// ���� Ŭ���� ��, �÷��̾���� ���� ���� ���� ���� ��Ŷ
struct SC_RANK_PACKET {
	char type;						// ��Ŷ Ÿ��
	int id[MAX_PLAYER];				// �÷��̾� id
	int score[MAX_PLAYER];			// �÷��̾� score
	char name[BUFSIZE][MAX_PLAYER];	// �÷��̾� ����
};
#pragma pack(pop) // ���� ������ ���� ����(�⺻��)�� �ǵ���

////////////////////////
// Ŭ���̾�Ʈ �� ���� //
////////////////////////
enum class CS_PACKET_TYPE {
	SELECT_CHARACTER = 1,
	CS_INIT_FINISH,
	CS_KEYBOARD_INPUT,
	CS_SELECT_LOBBY,
	CS_SELECT_EXIT,
};

#pragma pack(push, 1) // 1����Ʈ ũ��� ����
// ĳ���� ���� ���� ���� ��Ŷ
struct SELECT_CHARACTER_PACKET {
	char type;					// ��Ŷ Ÿ��
	int id;						// id
	CHARACTER_TYPE character;	// ĳ����
};

// �ʱ�ȭ �Ϸ� ���� ���� ��Ŷ
struct CS_INIT_FINISH_PACKET {
	char type;	// ��Ŷ Ÿ��
	int id;		// id
};

// Ű �Է� ���� ���� ��Ŷ
struct CS_KEYBOARD_INPUT_PACKET {
	char type;				// ��Ŷ Ÿ��
	KEY key;				// ���� Ű
	KEY_STATE key_state;	// ���� Ű�� ����
};

// �κ� ��ư ���� ���� ���� ��Ŷ
struct CS_SELECT_LOBBY_PACKET {
	char type;	// ��Ŷ Ÿ��
	int id;		// id
};

// ���� ��ư ���� ���� ���� ��Ŷ
struct CS_SELECT_EXIT_PACKET {
	char type;	// ��Ŷ Ÿ��
	int id;		// id
};
#pragma pack(pop) // ���� ������ ���� ����(�⺻��)�� �ǵ���