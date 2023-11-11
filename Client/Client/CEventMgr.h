#pragma once

struct tEvent
{
	EVENT_TYPE	eEvent;		// 이벤트
	DWORD_PTR	lParam;		// 부가정보
	DWORD_PTR	wParam;		// 부가정보
};

class CObject;

class CEventMgr
{
	SINGLE(CEventMgr)
private:
	std::vector<tEvent>		m_vecEvent;		// 이벤트 보관
	std::vector<CObject*>	m_vecDead;		// 삭제 예정 오브젝트 보관

public:
	void update();

private:
	void Process(const tEvent& _event);

public:
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }	// 이벤트 추가
};