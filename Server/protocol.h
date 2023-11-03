#include "Global.h"

#pragma once 
/////////////////////
// ���� �� Ŭ���̾�Ʈ //
////////////////////


// ��Ŷ Ÿ�� ���� 
enum class SC_PACKET_TYPE {
	SC_MAKE_ID=1,
	SELECT_CHARACTER,
	SC_INIT,
	SC_GAME_START,
	SC_GAME_OVER,
	SC_GAME_CLEAR,
	SC_PLAYER,
	SC_MONSTER,
	SC_BOSS,
	SC_BULLET,
	SC_ITEM,
	SC_RANK,
};


// ID �Ҵ� ���� ��Ŷ 
typedef struct SC_MAKE_ID_PACKET {
	char type; // ��Ŷ Ÿ��
	int id; // �Ҵ��� id(1p, 2p, 3p)
};

// �ʱ�ȭ ��ȣ ���� ���� ��Ŷ 
typedef struct SC_INIT_PACKET {
	char type; // ��Ŷ Ÿ��
};

// ���� ���� ��ȣ ���� ���� ��Ŷ 
typedef struct SC_GAME_START_PACKET {
	char type; // ��Ŷ Ÿ��
};

// ���� ���� ��ȣ ���� ���� ��Ŷ 
typedef struct SC_GAME_OVER_PACKET {
	char type; // ��Ŷ Ÿ��
};

// ���� Ŭ���� ��ȣ ���� ��Ŷ 
typedef struct SC_GAME_CLEAR_PACKET {
	char type; // ��Ŷ Ÿ��
};

// �÷��̾� ���� ���� ���� ��Ŷ 
typedef struct SC_PLAYER_PACKET {
	char type; // ��Ŷ Ÿ��
	CPlayer  player; // �÷��̾� ����
};

// ���� ���� ���� ���� ��Ŷ 
typedef struct SC_MONSTER_PACKET {
	char type; // ��Ŷ Ÿ��
	CMonster  monster; //   ���� ����
};

// ���� ���� ���� ���� ��Ŷ 
typedef struct SC_BOSS_PACKET {
	char type; // ��Ŷ Ÿ��
	CBoss  boss; // ���� ����
};

// ����ü ���� ���� ���� ��Ŷ 
typedef struct SC_BULLET_PACKET {
	char type; // ��Ŷ Ÿ��
	CBullet  bullet; // ����ü ����
};

// ������ ���� ���� ���� ��Ŷ 
typedef struct SC_ITEM_PACKET {
	char type; // ��Ŷ Ÿ��
	Citem  item; // ����ü ����
};

// ���� Ŭ���� ��, �÷��̾���� ���� ���� ���� ���� ��Ŷ 
typedef struct SC_RANK_PACKET {
	char type; // ��Ŷ Ÿ��
	int id[MAX_PLAYER]; // �÷��̾� id
	int score[MAX_PLAYER]; // �÷��̾� score
	char name[BUFSIZE][MAX_PLAYER];
	// �÷��̾� ����
};

/////////////////////
// Ŭ���̾�Ʈ �� ���� //
////////////////////


enum class CS_PACKET_TYPE {
	SELECT_CHARACTER = 1,
	CS_INIT_FINISH,
	CS_KEYBOARD_INPUT,
	CS_SELECT_LOBBY,
	CS_SELECT_EXIT,
};

// ĳ���� ���� ���� ���� ��Ŷ 
typedef struct SELECT_CHARACTER_PACKET {
	char type; // ��Ŷ Ÿ��
	int id; // id
	CHARACTER_TYPE character; // ĳ����
};

// �ʱ�ȭ �Ϸ� ���� ���� ��Ŷ 
typedef struct CS_INIT_FINISH_PACKET {
	char type; // ��Ŷ Ÿ��
	int id; // id
};

// Ű �Է� ���� ���� ��Ŷ 
typedef struct CS_KEYBOARD_INPUT_PACKET {
	char type; // ��Ŷ Ÿ��
	KEY key; // ���� Ű
	KEY_STATE key_state; // ���� Ű�� ����
	// 1. Ű�� �� ������ �� (TAP)
	// 2. Ű�� ��������  �� (HOLD)
	// 3. Ű�� ������ ��  (AWAY)
};

// �κ� ��ư ���� ���� ���� ��Ŷ 
typedef struct CS_SELECT_LOBBY_PACKET {
	char type; // ��Ŷ Ÿ��
	int id; // id
};

// ���� ��ư ���� ���� ���� ��Ŷ 
typedef struct CS_SELECT_EXIT_PACKET {
	char type; // ��Ŷ Ÿ��
	int id; // id
};
