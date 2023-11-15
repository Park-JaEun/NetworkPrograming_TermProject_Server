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
	// 부모 클래스로부터 받아온 함수들, virtual 키워드가 없어도 이미 가상 함수임.
	void Enter() override;
	void Exit() override;
	void update() override;

public:
	CScene_Main();
	~CScene_Main() override;
};

