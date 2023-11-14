#include "pch.h"
#include "CSceneMgr.h"

#include "CScene_Main.h"
#include "CScene_Start.h"

CSceneMgr::CSceneMgr() : m_arrScene{}, m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	// ��� Scene ����
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i) {
		if (m_arrScene[i] != nullptr) {
			delete m_arrScene[i];
		}
	}
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNextScene)
{
	// ���� Scene�� Exit �Լ� ȣ�� (������ �۾�)
	m_pCurScene->Exit();

	// Scene ����
	m_pCurScene = m_arrScene[(UINT)_eNextScene];

	// ���ο� Scene�� Enter �Լ� ȣ��
	m_pCurScene->Enter();
}

void CSceneMgr::init()
{
	// Scene ����

	// Start Scene
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");
	
	// ���� Stage Scene
	m_arrScene[(UINT)SCENE_TYPE::MAIN] = new CScene_Main;
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetName(L"Main Scene");

	// ó�� �� ����
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];
	m_pCurScene->Enter();
}

void CSceneMgr::update()
{
	m_pCurScene->update();

	m_pCurScene->finalUpdate();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}
