#include "pch.h"
#include "CSceneMgr.h"

#include "CScene_Main.h"
#include "CScene_Start.h"

CSceneMgr::CSceneMgr() : m_arrScene{}, m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	// 모든 Scene 삭제
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i) {
		if (m_arrScene[i] != nullptr) {
			delete m_arrScene[i];
		}
	}
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNextScene)
{
	// 현재 Scene의 Exit 함수 호출 (마무리 작업)
	m_pCurScene->Exit();

	// Scene 변경
	m_pCurScene = m_arrScene[(UINT)_eNextScene];

	// 새로운 Scene의 Enter 함수 호출
	m_pCurScene->Enter();
}

void CSceneMgr::init()
{
	// Scene 생성

	// Start Scene
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");
	
	// 메인 Stage Scene
	m_arrScene[(UINT)SCENE_TYPE::MAIN] = new CScene_Main;
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetName(L"Main Scene");

	// 처음 씬 지정
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
