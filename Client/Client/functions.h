#pragma once

class CObject;

// 오브젝트 생성
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateMonster(const Vec2& _vPos, int id);
void CreateFloor(const Vec2& _vPos);
void CreateBunny(const Vec2& _vPos, int id);
void CreateCookie(const Vec2& _vPos, int id);

// 오브젝트 삭제
void DeleteObject(CObject* _pObj);

// 씬 변경
void ChangeScene(SCENE_TYPE _eNextScene);

// 오브젝트가 게임 월드 내부에 존재하는지 확인
bool IsInWorld(const Vec2& _vPos);

// 오브젝트가 보스 방 내부에 존재하는지 확인
bool IsInBossRoom(const Vec2& _vPos);

// 오브젝트가 카메라 내부에 존재하는지 확인
bool IsInCamera(const Vec2& _vPos);

// 서버 통신 오류 함수
void err_quit(const char* msg);

// 소켓 함수 오류 출력
void err_display(const char* msg);

// 포지션 보간 함수
float LerpX(float start, float end, float alpha);
float LerpY(float start, float end, float alpha);


// send / recv
void setId(HWND hEditNickname, HWND hEditIP);	// 플레이어 ID를 설정하는 함수 

// send
void sendKeyBoardInput(SOCKET sock);		// 키 입력 정보 전송
void sendLobbySignal(SOCKET sock);			// 서버에게 로비 신호를 송신하는 함수
void sendExitSignal(SOCKET sock);			// 서버에게 종료 신호를 송신하는 함수
void sendInitFinishSignal(SOCKET sock);		// 서버에게 초기화 완료 신호를 송신하는 함수
void sendSelectCharacter(SOCKET sock);		//서버에게 캐릭터 선택 정보를 송신하는 함수 

// recv
bool recvInitSignal(SOCKET sock);			// 서버로부터 초기화 신호를 수신하는 함수
void recvPlayerInfo(SOCKET sock);			// 플레이어 정보 받기
void recvMonsterInfo(SOCKET sock);			// 몬스터 정보 받기
void recvBossInfo(SOCKET sock);				// 보스 정보 받기
void recvRabbitItemInfo(SOCKET sock);		// 토끼 아이템 정보 받기
void recvCookieInfo(SOCKET sock);			// 쿠키 정보 받기
void recvPlayerBulletInfo(SOCKET sock);		// 플레이어 투사체 정보 받기
void recvMonsterBulletInfo(SOCKET sock);	// 몬스터 투사체 정보 받기
void recvBossBulletInfo(SOCKET sock);		// 보스 투사체 정보 받기
void recvGameStateSignal(SOCKET sock);		// 게임 상태 패킷 받기