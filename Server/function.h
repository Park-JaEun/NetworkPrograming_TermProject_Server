#pragma once

class CObject;

// 오브젝트 생성
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateBoss();

void CreateMonster(const Vec2& _vPos);
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
// 소켓 함수 오류 출력
void err_display(const char* msg);