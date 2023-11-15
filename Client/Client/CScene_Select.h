#pragma once
#include "CScene.h"

class CScene_Select :
	public CScene
{
	CHARACTER_TYPE m_eSelectedCharacter;

public:
	CHARACTER_TYPE GetSelectedCharacter() { return m_eSelectedCharacter; }

public:
	// �θ� Ŭ�����κ��� �޾ƿ� �Լ���, virtual Ű���尡 ��� �̹� ���� �Լ���.
	void update() override;
	void Enter() override;
	void Exit() override;

public:
	CScene_Select();
	~CScene_Select() override;
};

