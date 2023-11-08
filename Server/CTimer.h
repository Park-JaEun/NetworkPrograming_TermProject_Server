#pragma once

class CTimer {
	SINGLE(CTimer)
private:
	LARGE_INTEGER	m_llCurCount;	// ���� ī��Ʈ
	LARGE_INTEGER	m_llPrevCount;	// ���� ī��Ʈ
	LARGE_INTEGER	m_llFrequency;	// �ʴ� ī��Ʈ Ƚ��

	float			m_fDT;			// ������ �� �ð�

public:
	void init();
	void update();

public:
	float GetDT() { return m_fDT; }
};