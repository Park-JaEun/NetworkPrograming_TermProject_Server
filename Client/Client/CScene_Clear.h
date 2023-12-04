#pragma once
#include "CScene.h"

class CScene_Clear : public CScene
{
public:
	// 부모 클래스로부터 받아온 함수들, virtual 키워드가 없어도 이미 가상 함수임.
	void update() override;
	void Enter() override;
	void Exit() override;
	void render(HDC _dc) override;

public:
	CScene_Clear();
	~CScene_Clear() override;
};

