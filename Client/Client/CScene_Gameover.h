#pragma once
#include "CScene.h"

class CScene_Gameover : public CScene
{

public:
	void update() override;
	void Enter() override;
	void Exit() override;
	void render(HDC _dc) override;

public:
	CScene_Gameover();
	~CScene_Gameover() override;
};

