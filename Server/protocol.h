#include "Global.h"

#pragma once 
/////////////////////
// 서버 → 클라이언트 //
////////////////////


// 패킷 타입 종류 
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


// ID 할당 관련 패킷 
typedef struct SC_MAKE_ID_PACKET {
	char type; // 패킷 타입
	int id; // 할당할 id(1p, 2p, 3p)
};

// 초기화 신호 수신 관련 패킷 
typedef struct SC_INIT_PACKET {
	char type; // 패킷 타입
};

// 게임 시작 신호 수신 관련 패킷 
typedef struct SC_GAME_START_PACKET {
	char type; // 패킷 타입
};

// 게임 오버 신호 수신 관련 패킷 
typedef struct SC_GAME_OVER_PACKET {
	char type; // 패킷 타입
};

// 게임 클리어 신호 관련 패킷 
typedef struct SC_GAME_CLEAR_PACKET {
	char type; // 패킷 타입
};

// 플레이어 정보 수신 관련 패킷 
typedef struct SC_PLAYER_PACKET {
	char type; // 패킷 타입
	CPlayer  player; // 플레이어 정보
};

// 몬스터 정보 수신 관련 패킷 
typedef struct SC_MONSTER_PACKET {
	char type; // 패킷 타입
	CMonster  monster; //   몬스터 정보
};

// 보스 정보 수신 관련 패킷 
typedef struct SC_BOSS_PACKET {
	char type; // 패킷 타입
	CBoss  boss; // 보스 정보
};

// 투사체 정보 수신 관련 패킷 
typedef struct SC_BULLET_PACKET {
	char type; // 패킷 타입
	CBullet  bullet; // 투사체 정보
};

// 아이템 정보 수신 관련 패킷 
typedef struct SC_ITEM_PACKET {
	char type; // 패킷 타입
	Citem  item; // 투사체 정보
};

// 게임 클리어 후, 플레이어들의 순위 정보 수신 관련 패킷 
typedef struct SC_RANK_PACKET {
	char type; // 패킷 타입
	int id[MAX_PLAYER]; // 플레이어 id
	int score[MAX_PLAYER]; // 플레이어 score
	char name[BUFSIZE][MAX_PLAYER];
	// 플레이어 별명
};

/////////////////////
// 클라이언트 → 서버 //
////////////////////


enum class CS_PACKET_TYPE {
	SELECT_CHARACTER = 1,
	CS_INIT_FINISH,
	CS_KEYBOARD_INPUT,
	CS_SELECT_LOBBY,
	CS_SELECT_EXIT,
};

// 캐릭터 선택 수신 관련 패킷 
typedef struct SELECT_CHARACTER_PACKET {
	char type; // 패킷 타입
	int id; // id
	CHARACTER_TYPE character; // 캐릭터
};

// 초기화 완료 수신 관련 패킷 
typedef struct CS_INIT_FINISH_PACKET {
	char type; // 패킷 타입
	int id; // id
};

// 키 입력 수신 관련 패킷 
typedef struct CS_KEYBOARD_INPUT_PACKET {
	char type; // 패킷 타입
	KEY key; // 눌린 키
	KEY_STATE key_state; // 눌린 키의 상태
	// 1. 키가 막 눌렸을 때 (TAP)
	// 2. 키가 눌려있을  때 (HOLD)
	// 3. 키를 놓았을 때  (AWAY)
};

// 로비 버튼 선택 수신 관련 패킷 
typedef struct CS_SELECT_LOBBY_PACKET {
	char type; // 패킷 타입
	int id; // id
};

// 종료 버튼 선택 수신 관련 패킷 
typedef struct CS_SELECT_EXIT_PACKET {
	char type; // 패킷 타입
	int id; // id
};
