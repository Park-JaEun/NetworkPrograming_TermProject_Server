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