#pragma once

class CTimer {
	SINGLE(CTimer)
private:
	LARGE_INTEGER	m_llCurCount;	// 현재 카운트
	LARGE_INTEGER	m_llPrevCount;	// 이전 카운트
	LARGE_INTEGER	m_llFrequency;	// 초당 카운트 횟수

	float			m_fDT;			// 프레임 간 시간

public:
	void init();
	void update();

public:
	float GetDT() { return m_fDT; }
};