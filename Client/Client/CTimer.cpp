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

	// ���� ������ ī��Ʈ, ���� ������ ī��Ʈ ���� ���̸� ���ϰ�, 1�ʴ� ī���� Ƚ���� ������ Delta Time�� ���Ѵ�.
	m_fDT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// ī��Ʈ �� ����
	m_llPrevCount = m_llCurCount;

#ifdef _DEBUG
	if (m_fDT > (1.f / 60.f))
		m_fDT = (1.f / 60.f);
#endif
}

void CTimer::render()
{
	++m_iCallCount;
	m_fAcc += m_fDT; // �ð� ����

	if (m_fAcc >= 1.f)
	{
		m_iFPS = m_iCallCount;
		m_fAcc = 0.f;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		wsprintf(szBuffer, L"FPS : %d", m_iFPS);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);	// Ÿ��Ʋ �ٿ� render
	}
}
