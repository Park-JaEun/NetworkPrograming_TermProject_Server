#pragma once
#include "define.h"
#include "structs.h"
#include "enums.h"
#include "functions.h"
#include "CBoss.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CBullet.h"
#include "CItem.h"

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
	SC_GAME_PROGRESS,
	SC_PLAYER,
	SC_MONSTER,
	SC_BOSS,
	SC_BULLET,
	SC_ITEM,
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

// ���� ������ ��ȣ ���� ��Ŷ (2023. 12. 04 ����)
struct SC_GAME_PROGRESS_PACKET {
	char type; // ��Ŷ Ÿ��
};

// �÷��̾� ���� ���� ���� ��Ŷ
struct SC_PLAYER_PACKET {
	char type;			// ��Ŷ Ÿ��
	// �÷��̾� ����
	int playerID;					// �÷��̾� id (2023. 12. 01 ����)
	Vec2 playerPos;					// �÷��̾� ��ġ
	PLAYER_STATE playerState;		// �÷��̾� ����
	bool playerDir;					// �÷��̾� ����
	int playerLife;					// �÷��̾� ��� (2023. 12. 02 ����)
	int playerHP;					// �÷��̾� ü�� (2023. 12. 02 ����)
	int playerBunnyCount;			// �÷��̾� �䳢 ȹ�� ���� (2023. 12. 02 ����)
	int playerCookieCount;			// �÷��̾� ��Ű ȹ�� ���� (2023. 12. 02 ����)
	int playerKillCount;			// �÷��̾� ų ���� (2023. 12. 02 ����)
	CHARACTER_TYPE character;		// ĳ���� (2023. 11. 29 ����)
};

// ���� ���� ���� ���� ��Ŷ
struct SC_MONSTER_PACKET {
	char type;			// ��Ŷ Ÿ��
	// ���� ����	(2023. 11. 30 ����)
	int monsterID;					// ���� id (2023. 12. 02 ����)
	Vec2 monsterPos;				// ���� ��ġ
	MONSTER_STATE monsterState;		// ���� ����
	bool monsterDir;				// ���� ����
	bool monsterIsDead;				// ���� ���� ���� (2023. 12. 02 ����)
};

// ���� ���� ���� ���� ��Ŷ
struct SC_BOSS_PACKET {
	char type;		// ��Ŷ Ÿ��
	// ���� ����
	Vec2 bossPos;					// ���� ��ġ (2023. 12. 01 ����)
	BOSS_STATE bossState;			// ���� ���� (2023. 12. 01 ����)
	bool bossIsDead;				// ���� ���� ���� (2023. 12. 01 ����)
};


// ����ü ���� ���� ���� ��Ŷ
struct SC_BULLET_PACKET {
	char type;			// ��Ŷ Ÿ��
	int playerID;		// �߻��� �÷��̾� id (2023. 12. 01 ����)
	int bulletID;		// ����ü id (2023. 12. 01 ����)
	Vec2 bulletPos;		// ����ü ��ġ (2023. 11. 30 ����)
	float bulletDir;	// ����ü ���� (2023. 12. 01 ����)
	float bulletDegree;	// ����ü ���� (2023. 12. 01 ����)
	bool bulletIsDead;	// ����ü ���� ���� (2023. 11. 30 ����)
};

// �䳢 ������ ���� ���� ���� ��Ŷ (2023. 12. 01 ����)
struct SC_RABBIT_ITEM_PACKET {
	char type;			// ��Ŷ Ÿ��
	int itemID;			// ������ id (2023. 12. 01 ����)
	Vec2 itemPos;		// ������ ��ġ(2023. 11. 30 ����)
	bool itemIsDead;	// ������ ���� ���� (2023. 11. 30 ����)
};

// ��Ű ������ ���� ���� ���� ��Ŷ (2023. 12. 01 ����)
struct SC_COOKIE_ITEM_PACKET {
	char type;			// ��Ŷ Ÿ��
	int itemID;			// ������ id (2023. 12. 01 ����)
	Vec2 itemPos;		// ������ ��ġ(2023. 11. 30 ����)
	bool itemIsDead;	// ������ ���� ���� (2023. 11. 30 ����)
};

// ���� Ŭ���� ��, �÷��̾���� ���� ���� ���� ���� ��Ŷ
struct SC_RANK_PACKET {
	char type;						// ��Ŷ Ÿ��
	int id[MAX_PLAYER];				// �÷��̾� id
	int score[MAX_PLAYER];			// �÷��̾� score0
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
	char type;
	int keyCount; // Ű �Է��� ����
	struct {
		KEY key;
		KEY_STATE key_state;
	} inputs[MAX_KEYS]; // MAX_KEYS�� ���ÿ� ó���� �� �ִ� �ִ� Ű �Է� ��
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