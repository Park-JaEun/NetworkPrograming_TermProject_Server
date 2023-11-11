#pragma once

class CAnimator;
class CTexture;

struct tAnimationFrame
{
	Vec2	vLT;		// LeftTop
	Vec2	vFrameSize;	// 프레임 한 장 사이즈
	Vec2	vOffset;	// 상대적인 위치
	float	fDuration;	// 지연 시간
};

class CAnimation
{
private:
	std::wstring					m_strName;		// 애니메이션 이름
	CAnimator*						m_pAnimator;	// 가리키는 애니메이터
	CTexture*						m_pTexture;		// 가리키는 텍스처
	std::vector<tAnimationFrame>	m_vecFrame;		// 프레임들을 보관할 벡터
	int								m_iCurFrame;	// 현재 프레임
	float							m_fAccTime;		// 시간 누적값
	Vec2							m_vScale;		// 애니메이션 스케일
	bool							m_bFinish;		// 애니메이션 재생 완료 여부

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

