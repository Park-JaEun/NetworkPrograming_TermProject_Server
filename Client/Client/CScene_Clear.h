#pragma once
#include "CScene.h"

class CScene_Clear : public CScene
{
	bool m_bClearAniEnd;
	int m_iScore[MAX_PLAYER];

public:
	const bool GetClearAniEnd() { return m_bClearAniEnd; }

	void SetClearAniEnd() { m_bClearAniEnd = true; }

	void update() override;
	void Enter() override;
	void Exit() override;
	void render(HDC _dc) override;

public:
	CScene_Clear();
	~CScene_Clear() override;
};

