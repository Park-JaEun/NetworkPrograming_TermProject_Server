#pragma once

class CObject;

// 오브젝트 생성
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateBoss();

void CreateMonster(const Vec2& _vPos, int id);
void CreateMonsters();

void CreateBunny(const Vec2& _vPos, int id);
void CreateCookie(const Vec2& _vPos, int id);
void CreateItems();

// 오브젝트 삭제
void DeleteObject(CObject* _pObj);

// 오브젝트가 게임 월드 내부에 존재하는지 확인
bool IsInWorld(const Vec2& _vPos);

// 오브젝트가 보스 방 내부에 존재하는지 확인
bool IsInBossRoom(const Vec2& _vPos);

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);
void err_display(const char* msg);


// 클라이언트 아이디 할당 및 송신
void sendPlayerId(int retval, SOCKET sock, int id, int size);

// 캐릭터 선택 정보를 수신하는 함수
int recvSelectCharacter(int retval, SOCKET sock, int size, char* buf);

// 초기화 신호 송신 함수
int sendInitSignal(int retval, int size, SOCKET sock);

// 초기화 완료 신호를 수신하는 함수
int recvInitFinishSignal(int retval, int size, SOCKET sock);

// 몬스터 정보 송신 함수
void sendMonsterInfo(SOCKET sock, int size, int retval);

// 보스 정보 송신 함수
void sendBossInfo(SOCKET sock, int size, int retval, bool& isBoss, bool& IsGameClear);

// 토끼 아이템 정보 송신
void sendRabbitInfo(SOCKET sock, int size, int retval);

// 쿠키 아이템 정보 송신
void sendCookieInfo(SOCKET sock, int size, int retval);

// 플레이어 투사체 정보 송신
void sendPlayerCopiedInfo(SOCKET sock, int size, int retval);

// 몬스터 투사체 정보 송신
void sendMonsterCopiedInfo(SOCKET sock, int size, int retval);

// 보스 투사체 정보 송신
void sendBossCopiedInfo(SOCKET sock, int size, int retval);

// 플레이어 랭킹을 계산하고 클라이언트에게 게임 클리어 정보와 순위 정보를 송신
void sendRankInfo(SOCKET sock, int retval, int size, int id);

// 로비 신호와 종료 신호를 수신하는 함수
int recvSignal(SOCKET sock, char* buf, int retval, int size, bool& isLobby, bool& IsExit);

