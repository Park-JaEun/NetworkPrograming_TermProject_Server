#include "pch.h"

#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CObject.h"

CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::update()
{
	// ���� ���� ������Ʈ�� ����
	for (CObject* pDeadObj : m_vecDead)
		delete pDeadObj;

	// clear
	if (!m_vecDead.empty())
		m_vecDead.clear();

	// �̺�Ʈ ó�� (Process�Լ� ������ �߰��� ��ҵ��� �������� �ʴ� ������ �ذ� 23.11.02)
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
		// ������Ʈ �߰�
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam: Object Adress
		// wParam: Group Type

		CObject* pNewObj = (CObject*)_event.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_event.wParam;

		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	}
		break;

		// ������Ʈ�� Dead ���·� ���� (���� ���� ����)
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam: Object Adress
		// wParam: UNREFERENCED PARAMETER

		CObject* pDeadObj = (CObject*)_event.lParam;

		if (!pDeadObj->IsDead())		// ��� ������ ���� ���� ���·� ����
			pDeadObj->SetDead();

		m_vecDead.push_back(pDeadObj);	// dead ���Ϳ� ����
	}
		break;

	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam: Next Scene Type
		// wParam: UNREFERENCED PARAMETER

		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_event.lParam);
	}
		break;

	}
}
