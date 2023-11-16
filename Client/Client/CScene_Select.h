#pragma once
#include "CScene.h"

class CScene_Select :
	public CScene
{
	CHARACTER_TYPE m_eSelectedCharacter;

public:
	CHARACTER_TYPE GetSelectedCharacter() { return m_eSelectedCharacter; }

public:
	// 부모 클래스로부터 받아온 함수들, virtual 키워드가 없어도 이미 가상 함수임.
	void update() override;
	void Enter() override;
	void Exit() override;

public:
	CScene_Select();
	~CScene_Select() override;
};

