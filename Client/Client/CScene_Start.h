#pragma once
#include "CScene.h"

class CScene_Start :
    public CScene
{
	HWND hEditNickname; // 닉네임을 위한 Edit 컨트롤 핸들
	HWND hEditIP;       // IP 주소를 위한 Edit 컨트롤 핸들

public:
	// 부모 클래스로부터 받아온 함수들, virtual 키워드가 없어도 이미 가상 함수임.
	void update() override;
	void Enter() override;
	void Exit() override;
	void render(HDC _dc) override;

	void EditBoxRender(HDC _dc);

public:
	CScene_Start();
	~CScene_Start() override;
};

