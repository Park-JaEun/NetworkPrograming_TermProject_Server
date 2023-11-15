#pragma once
#include "CScene.h"

class CScene_Start :
    public CScene
{
	HWND hEditNickname; // �г����� ���� Edit ��Ʈ�� �ڵ�
	HWND hEditIP;       // IP �ּҸ� ���� Edit ��Ʈ�� �ڵ�

public:
	// �θ� Ŭ�����κ��� �޾ƿ� �Լ���, virtual Ű���尡 ��� �̹� ���� �Լ���.
	void update() override;
	void Enter() override;
	void Exit() override;
	void render(HDC _dc) override;

	void EditBoxRender(HDC _dc);

public:
	CScene_Start();
	~CScene_Start() override;
};

