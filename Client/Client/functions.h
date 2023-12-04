#pragma once

class CObject;

// ������Ʈ ����
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateMonster(const Vec2& _vPos, int id);
void CreateFloor(const Vec2& _vPos);
void CreateBunny(const Vec2& _vPos, int id);
void CreateCookie(const Vec2& _vPos, int id);

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

// ���� ��� ���� �Լ�
void err_quit(const char* msg);

// ���� �Լ� ���� ���
void err_display(const char* msg);

// ������ ���� �Լ�
float LerpX(float start, float end, float alpha);
float LerpY(float start, float end, float alpha);