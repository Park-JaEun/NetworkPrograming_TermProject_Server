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
	// ���� �ִ� �������� �����Ǿ� �ִٸ�, m_fDT�� 1.f / m_iMaxFrame���� �����Ѵ�.
	if (MAX_FRAME != 0) {
		m_fDT = 1.f / MAX_FRAME;
	}
	else  {	// �׷��� �ʴٸ�, �������� ����Ѵ�.
		QueryPerformanceCounter(&m_llCurCount);

		// ���� ������ ī��Ʈ, ���� ������ ī��Ʈ ���� ���̸� ���ϰ�, 1�ʴ� ī���� Ƚ���� ������ Delta Time�� ���Ѵ�.
		m_fDT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

		// ī��Ʈ �� ����
		m_llPrevCount = m_llCurCount;
	}
}