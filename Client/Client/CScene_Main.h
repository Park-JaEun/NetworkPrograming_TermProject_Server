#pragma once
#include "CScene.h"

class CScene_Main : 
	public CScene
{
	bool m_bIsBoss;

public:
	void SetIsBoss(bool _bIsBoss) { m_bIsBoss = _bIsBoss; }
	bool GetIsBoss() { return m_bIsBoss; }

public:
	// 부모 클래스로부터 받아온 함수들, virtual 키워드가 없어도 이미 가상 함수임.
	void Enter() override;
	void Exit() override;
	void update() override;

public:
	CScene_Main();
	~CScene_Main() override;
};

