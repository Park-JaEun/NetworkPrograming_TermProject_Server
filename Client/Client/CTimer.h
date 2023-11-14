#pragma once

class CTimer
{
	SINGLE(CTimer)
private:
	LARGE_INTEGER	m_llCurCount;	// 현재 카운트
	LARGE_INTEGER	m_llPrevCount;	// 이전 카운트
	LARGE_INTEGER	m_llFrequency;	// 초당 카운트 횟수

	float			m_fDT;			// 프레임 간 시간
	float			m_fAcc;			// 1초 체크를 위한 시간 누적값
	UINT			m_iCallCount;	// 초당 호출 횟수
	UINT			m_iFPS;			// 1초간 그리는 횟수

public:
	void init();
	void update();
	void render();

public:
	float GetDT()	{ return m_fDT; }
};