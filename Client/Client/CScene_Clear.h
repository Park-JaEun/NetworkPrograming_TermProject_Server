#pragma once
#include "CScene.h"

class CScene_Clear : public CScene
{
public:
	// �θ� Ŭ�����κ��� �޾ƿ� �Լ���, virtual Ű���尡 ��� �̹� ���� �Լ���.
	void update() override;
	void Enter() override;
	void Exit() override;
	void render(HDC _dc) override;

public:
	CScene_Clear();
	~CScene_Clear() override;
};

