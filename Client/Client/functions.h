#pragma once

class CObject;

// 오브젝트 생성
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateMonster(const Vec2& _vPos);
void CreateFloor(const Vec2& _vPos);

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