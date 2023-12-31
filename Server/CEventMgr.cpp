#include "pch.h"

#include "CEventMgr.h"

#include "CObject.h"
#include "CObjectMgr.h"

//std::mutex g_mutex_event;

CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::update()
{
	//std::lock_guard<std::mutex> lock{ g_mutex_event };

	// 삭제 예정 오브젝트들 삭제
	for (CObject* pDeadObj : m_vecDead) {
		pDeadObj = nullptr;
		delete pDeadObj;
	}

	// clear
	if (!m_vecDead.empty())
		m_vecDead.clear();

	// 이벤트 처리 (Process함수 내에서 추가된 요소들은 실행하지 않는 문제점 해결 23.11.02)
	while (!m_vecEvent.empty()) {
		tEvent event = m_vecEvent.front();
		m_vecEvent.erase(m_vecEvent.begin());
		Process(event);
	}
}

void CEventMgr::Process(const tEvent& _event)
{
	switch (_event.eEvent)
	{
		// 오브젝트 추가
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam: Object Adress
		// wParam: Group Type

		CObject* pNewObj = (CObject*)_event.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_event.wParam;

		CObjectMgr::GetInst()->AddObject(pNewObj, eType);
	}
	break;

	// 오브젝트를 Dead 상태로 변경 (삭제 예정 상태)
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam: Object Adress
		// wParam: UNREFERENCED PARAMETER

		CObject* pDeadObj = (CObject*)_event.lParam;

		if (!pDeadObj->IsDead()) {	// 살아 있으면 삭제 예정 상태로 만듬
			pDeadObj->SetDead();
			m_vecDead.push_back(pDeadObj);	// dead 벡터에 넣음
		}
	}
	break;

	}
}
