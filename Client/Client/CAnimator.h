#pragma once

class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	std::map<std::wstring, CAnimation*>		m_mapAnimation;		// �ִϸ��̼ǵ��� ������ ��
	CAnimation*								m_pCurAnimation;	// ���� ������� �ִϸ��̼�
	CObject*								m_pObject;			// ����Ű�� ������Ʈ
	bool									m_bRepeat;			// �ݺ� ��� ����

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

