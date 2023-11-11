#pragma once

class CObject;

// ������Ʈ ����
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateMonster(const Vec2& _vPos);
void CreateFloor(const Vec2& _vPos);

// ������Ʈ ����
void DeleteObject(CObject* _pObj);

// �� ����
void ChangeScene(SCENE_TYPE _eNextScene);

// ������Ʈ�� ���� ���� ���ο� �����ϴ��� Ȯ��
bool IsInWorld(const Vec2& _vPos);

// ������Ʈ�� ���� �� ���ο� �����ϴ��� Ȯ��
bool IsInBossRoom(const Vec2& _vPos);

// ������Ʈ�� ī�޶� ���ο� �����ϴ��� Ȯ��
bool IsInCamera(const Vec2& _vPos);