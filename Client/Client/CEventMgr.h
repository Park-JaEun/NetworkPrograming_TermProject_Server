#pragma once

struct tEvent
{
	EVENT_TYPE	eEvent;		// �̺�Ʈ
	DWORD_PTR	lParam;		// �ΰ�����
	DWORD_PTR	wParam;		// �ΰ�����
};

class CObject;

class CEventMgr
{
	SINGLE(CEventMgr)
private:
	std::vector<tEvent>		m_vecEvent;		// �̺�Ʈ ����
	std::vector<CObject*>	m_vecDead;		// ���� ���� ������Ʈ ����

public:
	void update();

private:
	void Process(const tEvent& _event);

public:
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }	// �̺�Ʈ �߰�
};