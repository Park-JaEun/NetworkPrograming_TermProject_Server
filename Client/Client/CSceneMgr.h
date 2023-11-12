#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	std::array<CScene*, (UINT)SCENE_TYPE::END>	m_arrScene;		// Scene���� ������ �迭
	CScene*										m_pCurScene;	// ���� ��

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

