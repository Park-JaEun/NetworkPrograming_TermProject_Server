#pragma once
#include "CScene.h"

class CScene_Start :
    public CScene
{
public:
	// �θ� Ŭ�����κ��� �޾ƿ� �Լ���, virtual Ű���尡 ��� �̹� ���� �Լ���.
	void update() override;
	void Enter() override;
	void Exit() override;

public:
	CScene_Start();
	~CScene_Start() override;
};

