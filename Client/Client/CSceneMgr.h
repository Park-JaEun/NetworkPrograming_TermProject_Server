#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	std::array<CScene*, (UINT)SCENE_TYPE::END>	m_arrScene;		// Scene들을 관리할 배열
	CScene*										m_pCurScene;	// 현재 씬

public:
	CScene* GetCurScene() { return m_pCurScene; }

public:
	void init();
	void update();
	void render(HDC _dc);

private:
	void ChangeScene(SCENE_TYPE _eNextScene);

	friend class CEventMgr;
};

