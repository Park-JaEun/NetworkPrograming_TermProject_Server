#pragma once
#include "define.h"
#include "struct.h"
#include "enum.h"
#include "function.h"
#include "CBoss.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CBullet.h"
#include "CItem.h"

////////////////////////
// 서버 → 클라이언트 //
////////////////////////

// 패킷 타입 종류
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
	SC_ITEM,
	SC_RANK,
};

#pragma pack(push, 1) // 1바이트 크기로 정렬
// ID 할당 관련 패킷
struct SC_MAKE_ID_PACKET {
	char type;	// 패킷 타입
	int id;		// 할당할 id(1p, 2p, 3p)
};

// 초기화 신호 수신 관련 패킷
struct SC_INIT_PACKET {
	char type; // 패킷 타입
};

// 게임 시작 신호 수신 관련 패킷
struct SC_GAME_START_PACKET {
	char type; // 패킷 타입
};

// 게임 오버 신호 수신 관련 패킷
struct SC_GAME_OVER_PACKET {
	char type; // 패킷 타입
};

// 게임 클리어 신호 관련 패킷
struct SC_GAME_CLEAR_PACKET {
	char type; // 패킷 타입
};

// 플레이어 정보 수신 관련 패킷
struct SC_PLAYER_PACKET {
	char type;			// 패킷 타입
	// 플레이어 정보
	Vec2 playerPos;					// 플레이어 위치
	PLAYER_STATE playerState;		// 플레이어 상태
	bool playerDir;					// 플레이어 방향
	CHARACTER_TYPE character;		// 캐릭터 (2023. 11. 29 수정)
};

// 몬스터 정보 수신 관련 패킷
struct SC_MONSTER_PACKET {
	char type;			// 패킷 타입
	// 몬스터 정보	(2023. 11. 30 수정)
	Vec2 monsterPos;					// 몬스터 위치
	MONSTER_STATE monsterState;		// 몬스터 상태
	bool monsterDir;					// 몬스터 방향
};

// 보스 정보 수신 관련 패킷
struct SC_BOSS_PACKET {
	char type;		// 패킷 타입
	CBoss  boss;	// 보스 정보
};

// 투사체 정보 수신 관련 패킷
struct SC_BULLET_PACKET {
	char type;			// 패킷 타입
	CBullet  bullet;	// 투사체 정보
};

// 아이템 정보 수신 관련 패킷
struct SC_ITEM_PACKET {
	char type;		// 패킷 타입
	CItem  item;	// 투사체 정보
};

// 게임 클리어 후, 플레이어들의 순위 정보 수신 관련 패킷
struct SC_RANK_PACKET {
	char type;						// 패킷 타입
	int id[MAX_PLAYER];				// 플레이어 id
	int score[MAX_PLAYER];			// 플레이어 score
	char name[BUFSIZE][MAX_PLAYER];	// 플레이어 별명
};
#pragma pack(pop) // 정렬 설정을 이전 상태(기본값)로 되돌림

////////////////////////
// 클라이언트 → 서버 //
////////////////////////
enum class CS_PACKET_TYPE {
	SELECT_CHARACTER = 1,
	CS_INIT_FINISH,
	CS_KEYBOARD_INPUT,
	CS_SELECT_LOBBY,
	CS_SELECT_EXIT,
};

#pragma pack(push, 1) // 1바이트 크기로 정렬
// 캐릭터 선택 수신 관련 패킷
struct SELECT_CHARACTER_PACKET {
	char type;					// 패킷 타입
	int id;						// id
	CHARACTER_TYPE character;	// 캐릭터
};

// 초기화 완료 수신 관련 패킷
struct CS_INIT_FINISH_PACKET {
	char type;	// 패킷 타입
	int id;		// id
};

// 키 입력 수신 관련 패킷
struct CS_KEYBOARD_INPUT_PACKET {
	char type;
	int keyCount; // 키 입력의 개수
	struct {
		KEY key;
		KEY_STATE key_state;
	} inputs[MAX_KEYS]; // MAX_KEYS는 동시에 처리할 수 있는 최대 키 입력 수
};


// 로비 버튼 선택 수신 관련 패킷
struct CS_SELECT_LOBBY_PACKET {
	char type;	// 패킷 타입
	int id;		// id
};

// 종료 버튼 선택 수신 관련 패킷
struct CS_SELECT_EXIT_PACKET {
	char type;	// 패킷 타입
	int id;		// id
};
#pragma pack(pop) // 정렬 설정을 이전 상태(기본값)로 되돌림