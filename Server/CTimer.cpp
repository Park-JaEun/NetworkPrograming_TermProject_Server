#include "pch.h"
#include "CTimer.h"

CTimer::CTimer() : m_llCurCount{}, m_llPrevCount{}, m_llFrequency{}, m_fDT(0.f)
{}

CTimer::~CTimer()
{}

void CTimer::init()
{
	QueryPerformanceCounter(&m_llPrevCount);
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimer::update()
{
	// 만약 최대 프레임이 설정되어 있다면, m_fDT를 1.f / m_iMaxFrame으로 제한한다.
	if (MAX_FRAME != 0) {
		m_fDT = 1.f / MAX_FRAME;
	}
	else  {	// 그렇지 않다면, 프레임을 계산한다.
		QueryPerformanceCounter(&m_llCurCount);

		// 이전 프레임 카운트, 현재 프레임 카운트 값의 차이를 구하고, 1초당 카운팅 횟수로 나눠서 Delta Time을 구한다.
		m_fDT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

		// 카운트 값 갱신
		m_llPrevCount = m_llCurCount;
	}
}