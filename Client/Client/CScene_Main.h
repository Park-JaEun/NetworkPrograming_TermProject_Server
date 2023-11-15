#pragma once
#include "CScene.h"

class CScene_Main : 
	public CScene
{
	CHARACTER_TYPE m_eSelectedCharacter;
	bool m_bIsBoss;

public:
	bool GetIsBoss() { return m_bIsBoss; }

	void SetIsBoss(bool _bIsBoss) { m_bIsBoss = _bIsBoss; }
	void SetSelectedCharacter(CHARACTER_TYPE _eCharacterType) { m_eSelectedCharacter = _eCharacterType; }

public:
	// �θ� Ŭ�����κ��� �޾ƿ� �Լ���, virtual Ű���尡 ��� �̹� ���� �Լ���.
	void Enter() override;
	void Exit() override;
	void update() override;

public:
	CScene_Main();
	~CScene_Main() override;
};

