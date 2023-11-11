#pragma once

class CTimer
{
	SINGLE(CTimer)
private:
	LARGE_INTEGER	m_llCurCount;	// ���� ī��Ʈ
	LARGE_INTEGER	m_llPrevCount;	// ���� ī��Ʈ
	LARGE_INTEGER	m_llFrequency;	// �ʴ� ī��Ʈ Ƚ��

	float			m_fDT;			// ������ �� �ð�
	float			m_fAcc;			// 1�� üũ�� ���� �ð� ������
	UINT			m_iCallCount;	// �ʴ� ȣ�� Ƚ��
	UINT			m_iFPS;			// 1�ʰ� �׸��� Ƚ��

public:
	void init();
	void update();
	void render();

public:
	float GetDT()	{ return m_fDT; }
};