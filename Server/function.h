#pragma once

class CObject;

// ������Ʈ ����
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateMonster(const Vec2& _vPos);

// ������Ʈ ����
void DeleteObject(CObject* _pObj);

// ������Ʈ�� ���� ���� ���ο� �����ϴ��� Ȯ��
bool IsInWorld(const Vec2& _vPos);

// ������Ʈ�� ���� �� ���ο� �����ϴ��� Ȯ��
bool IsInBossRoom(const Vec2& _vPos);

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);
// ���� �Լ� ���� ���
void err_display(const char* msg);