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


// send / recv
void setId(HWND hEditNickname, HWND hEditIP);	// �÷��̾� ID�� �����ϴ� �Լ� 

// send
void sendKeyBoardInput(SOCKET sock);		// Ű �Է� ���� ����
void sendLobbySignal(SOCKET sock);			// �������� �κ� ��ȣ�� �۽��ϴ� �Լ�
void sendExitSignal(SOCKET sock);			// �������� ���� ��ȣ�� �۽��ϴ� �Լ�
void sendInitFinishSignal(SOCKET sock);		// �������� �ʱ�ȭ �Ϸ� ��ȣ�� �۽��ϴ� �Լ�
void sendSelectCharacter(SOCKET sock);		//�������� ĳ���� ���� ������ �۽��ϴ� �Լ� 

// recv
bool recvInitSignal(SOCKET sock);			// �����κ��� �ʱ�ȭ ��ȣ�� �����ϴ� �Լ�
void recvPlayerInfo(SOCKET sock);			// �÷��̾� ���� �ޱ�
void recvMonsterInfo(SOCKET sock);			// ���� ���� �ޱ�
void recvBossInfo(SOCKET sock);				// ���� ���� �ޱ�
void recvRabbitItemInfo(SOCKET sock);		// �䳢 ������ ���� �ޱ�
void recvCookieInfo(SOCKET sock);			// ��Ű ���� �ޱ�
void recvPlayerBulletInfo(SOCKET sock);		// �÷��̾� ����ü ���� �ޱ�
void recvMonsterBulletInfo(SOCKET sock);	// ���� ����ü ���� �ޱ�
void recvBossBulletInfo(SOCKET sock);		// ���� ����ü ���� �ޱ�
void recvGameStateSignal(SOCKET sock);		// ���� ���� ��Ŷ �ޱ�