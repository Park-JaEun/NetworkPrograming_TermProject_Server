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
	// �θ� Ŭ�����κ��� �޾ƿ� �Լ���, virtual Ű���尡 ��� �̹� ���� �Լ���.
	void Enter() override;
	void Exit() override;
	void update() override;

public:
	CScene_Main();
	~CScene_Main() override;
};

