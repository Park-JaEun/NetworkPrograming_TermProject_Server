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
	SC_GAME_PROGRESS,
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

// 게임 진행중 신호 관련 패킷 (2023. 12. 04 수정)
struct SC_GAME_PROGRESS_PACKET {
	char type; // 패킷 타입
};

// 플레이어 정보 수신 관련 패킷
struct SC_PLAYER_PACKET {
	char type;			// 패킷 타입
	// 플레이어 정보
	int playerID;					// 플레이어 id (2023. 12. 01 수정)
	Vec2 playerPos;					// 플레이어 위치
	PLAYER_STATE playerState;		// 플레이어 상태
	bool playerDir;					// 플레이어 방향
	int playerLife;					// 플레이어 목숨 (2023. 12. 02 수정)
	int playerHP;					// 플레이어 체력 (2023. 12. 02 수정)
	int playerBunnyCount;			// 플레이어 토끼 획득 개수 (2023. 12. 02 수정)
	int playerCookieCount;			// 플레이어 쿠키 획득 개수 (2023. 12. 02 수정)
	int playerKillCount;			// 플레이어 킬 개수 (2023. 12. 02 수정)
	CHARACTER_TYPE character;		// 캐릭터 (2023. 11. 29 수정)
};

// 몬스터 정보 수신 관련 패킷
struct SC_MONSTER_PACKET {
	char type;			// 패킷 타입
	// 몬스터 정보	(2023. 11. 30 수정)
	int monsterID;					// 몬스터 id (2023. 12. 02 수정)
	Vec2 monsterPos;				// 몬스터 위치
	MONSTER_STATE monsterState;		// 몬스터 상태
	bool monsterDir;				// 몬스터 방향
	bool monsterIsDead;				// 몬스터 생존 여부 (2023. 12. 02 수정)
};

// 보스 정보 수신 관련 패킷
struct SC_BOSS_PACKET {
	char type;		// 패킷 타입
	// 보스 정보
	Vec2 bossPos;					// 보스 위치 (2023. 12. 01 수정)
	BOSS_STATE bossState;			// 보스 상태 (2023. 12. 01 수정)
	bool bossIsDead;				// 보스 생존 여부 (2023. 12. 01 수정)
};


// 투사체 정보 수신 관련 패킷
struct SC_BULLET_PACKET {
	char type;			// 패킷 타입
	int playerID;		// 발사한 플레이어 id (2023. 12. 01 수정)
	int bulletID;		// 투사체 id (2023. 12. 01 수정)
	Vec2 bulletPos;		// 투사체 위치 (2023. 11. 30 수정)
	float bulletDir;	// 투사체 방향 (2023. 12. 01 수정)
	float bulletDegree;	// 투사체 각도 (2023. 12. 01 수정)
	bool bulletIsDead;	// 투사체 생존 여부 (2023. 11. 30 수정)
};

// 토끼 아이템 정보 수신 관련 패킷 (2023. 12. 01 수정)
struct SC_RABBIT_ITEM_PACKET {
	char type;			// 패킷 타입
	int itemID;			// 아이템 id (2023. 12. 01 수정)
	Vec2 itemPos;		// 아이템 위치(2023. 11. 30 수정)
	bool itemIsDead;	// 아이템 생존 여부 (2023. 11. 30 수정)
};

// 쿠키 아이템 정보 수신 관련 패킷 (2023. 12. 01 수정)
struct SC_COOKIE_ITEM_PACKET {
	char type;			// 패킷 타입
	int itemID;			// 아이템 id (2023. 12. 01 수정)
	Vec2 itemPos;		// 아이템 위치(2023. 11. 30 수정)
	bool itemIsDead;	// 아이템 생존 여부 (2023. 11. 30 수정)
};

// 게임 클리어 후, 플레이어들의 순위 정보 수신 관련 패킷
struct SC_RANK_PACKET {
	char type;						// 패킷 타입
	int id[MAX_PLAYER];				// 플레이어 id
	int score[MAX_PLAYER];			// 플레이어 score0
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