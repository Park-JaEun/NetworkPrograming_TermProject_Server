#include "pch.h"
#include "CTimer.h"
#include "CCore.h"

CTimer::CTimer() : m_llCurCount{}, m_llPrevCount{}, m_llFrequency{}, m_fDT(0.f), m_iCallCount(0), m_fAcc(0.f)
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
	QueryPerformanceCounter(&m_llCurCount);

	// 이전 프레임 카운트, 현재 프레임 카운트 값의 차이를 구하고, 1초당 카운팅 횟수로 나눠서 Delta Time을 구한다.
	m_fDT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// 카운트 값 갱신
	m_llPrevCount = m_llCurCount;

#ifdef _DEBUG
	if (m_fDT > (1.f / 60.f))
		m_fDT = (1.f / 60.f);
#endif
}

void CTimer::render()
{
	++m_iCallCount;
	m_fAcc += m_fDT; // 시간 누적

	if (m_fAcc >= 1.f)
	{
		m_iFPS = m_iCallCount;
		m_fAcc = 0.f;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		wsprintf(szBuffer, L"FPS : %d", m_iFPS);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);	// 타이틀 바에 render
	}
}
