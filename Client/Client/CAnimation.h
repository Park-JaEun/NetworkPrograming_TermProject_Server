#pragma once

class CAnimator;
class CTexture;

struct tAnimationFrame
{
	Vec2	vLT;		// LeftTop
	Vec2	vFrameSize;	// ������ �� �� ������
	Vec2	vOffset;	// ������� ��ġ
	float	fDuration;	// ���� �ð�
};

class CAnimation
{
private:
	std::wstring					m_strName;		// �ִϸ��̼� �̸�
	CAnimator*						m_pAnimator;	// ����Ű�� �ִϸ�����
	CTexture*						m_pTexture;		// ����Ű�� �ؽ�ó
	std::vector<tAnimationFrame>	m_vecFrame;		// �����ӵ��� ������ ����
	int								m_iCurFrame;	// ���� ������
	float							m_fAccTime;		// �ð� ������
	Vec2							m_vScale;		// �ִϸ��̼� ������
	bool							m_bFinish;		// �ִϸ��̼� ��� �Ϸ� ����

public:
	void SetName(const std::wstring& _strName)	{ m_strName = _strName; }
	void SetAnimator(CAnimator* _pAnimator)		{ m_pAnimator = _pAnimator; }
	void SetScale(Vec2 _vScale)					{ m_vScale = _vScale; }
	void SetFrame(int _iFrameIdx)		
	{ 
		m_bFinish = false;
		m_iCurFrame = _iFrameIdx;
		m_fAccTime = 0.f;
	}

	const std::wstring&	GetName()				{ return m_strName; }
	CAnimator* GetAnimator()					{ return m_pAnimator; }
	bool IsFinish()								{ return m_bFinish; }
	tAnimationFrame& GetFrame(int _iIdx)		{ return m_vecFrame[_iIdx]; }
	int GetMaxFrame()							{ return (int)m_vecFrame.size(); }
	CTexture* GetTexture()						{ return m_pTexture; }

public:
	void update();
	void render(HDC _dc);

public:
	void Create(CTexture* _pTexture, const Vec2& _vLT, const Vec2& _vFrameSize, const Vec2& _vStep, float _fDuration, const Vec2& _vOffset, UINT _iFrameCount);

public:
	CAnimation();
	~CAnimation();
};

