#pragma once

class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	std::map<std::wstring, CAnimation*>		m_mapAnimation;		// 애니메이션들을 보관할 맵
	CAnimation*								m_pCurAnimation;	// 현재 재생중인 애니메이션
	CObject*								m_pObject;			// 가리키는 오브젝트
	bool									m_bRepeat;			// 반복 재생 여부

public:
	CAnimation* FindAnimation(const std::wstring& _strName);
	CObject* GetObj()				{ return m_pObject; }

	void SetObj(CObject* _pObject)	{ m_pObject = _pObject; }

public:
	void update();
	void render(HDC _dc);

public:
	void CreateAnimation(const std::wstring& _strName, CTexture* _pTexture, const Vec2& _vLT, const Vec2& _vFrameSize, const Vec2& _vStep, float _fDuration, const Vec2& _vOffset, UINT _iFrameCount);
	void CreateAnimation(const std::wstring& _strName, CTexture* _pTexture, const Vec2& _vLT, const Vec2& _vFrameSize, const Vec2& _vStep, float _fDuration, const Vec2& _vOffset, const Vec2& _vScale, UINT _iFrameCount);
	void Play(const std::wstring& _strName, bool _bRepeat);

public:
	CAnimator();
	~CAnimator();
};

