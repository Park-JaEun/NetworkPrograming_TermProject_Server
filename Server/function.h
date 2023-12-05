#pragma once

class CObject;

// ������Ʈ ����
void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void CreateBoss();

void CreateMonster(const Vec2& _vPos, int id);
void CreateMonsters();

void CreateBunny(const Vec2& _vPos, int id);
void CreateCookie(const Vec2& _vPos, int id);
void CreateItems();

// ������Ʈ ����
void DeleteObject(CObject* _pObj);

// ������Ʈ�� ���� ���� ���ο� �����ϴ��� Ȯ��
bool IsInWorld(const Vec2& _vPos);

// ������Ʈ�� ���� �� ���ο� �����ϴ��� Ȯ��
bool IsInBossRoom(const Vec2& _vPos);

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);
void err_display(const char* msg);


// Ŭ���̾�Ʈ ���̵� �Ҵ� �� �۽�
void sendPlayerId(int retval, SOCKET sock, int id, int size);

// ĳ���� ���� ������ �����ϴ� �Լ�
int recvSelectCharacter(int retval, SOCKET sock, int size, char* buf);

// �ʱ�ȭ ��ȣ �۽� �Լ�
int sendInitSignal(int retval, int size, SOCKET sock);

// �ʱ�ȭ �Ϸ� ��ȣ�� �����ϴ� �Լ�
int recvInitFinishSignal(int retval, int size, SOCKET sock);

// ���� ���� �۽� �Լ�
void sendMonsterInfo(SOCKET sock, int size, int retval);

// ���� ���� �۽� �Լ�
void sendBossInfo(SOCKET sock, int size, int retval, bool& isBoss, bool& IsGameClear);

// �䳢 ������ ���� �۽�
void sendRabbitInfo(SOCKET sock, int size, int retval);

// ��Ű ������ ���� �۽�
void sendCookieInfo(SOCKET sock, int size, int retval);

// �÷��̾� ����ü ���� �۽�
void sendPlayerCopiedInfo(SOCKET sock, int size, int retval);

// ���� ����ü ���� �۽�
void sendMonsterCopiedInfo(SOCKET sock, int size, int retval);

// ���� ����ü ���� �۽�
void sendBossCopiedInfo(SOCKET sock, int size, int retval);

// �÷��̾� ��ŷ�� ����ϰ� Ŭ���̾�Ʈ���� ���� Ŭ���� ������ ���� ������ �۽�
void sendRankInfo(SOCKET sock, int retval, int size, int id);

// �κ� ��ȣ�� ���� ��ȣ�� �����ϴ� �Լ�
int recvSignal(SOCKET sock, char* buf, int retval, int size, bool& isLobby, bool& IsExit);

