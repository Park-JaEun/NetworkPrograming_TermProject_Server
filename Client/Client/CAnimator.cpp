#include "pch.h"

#include "CAnimator.h"
#include "CAnimation.h"

CAnimator::CAnimator() : m_mapAnimation{}, m_pCurAnimation(nullptr), m_pObject(nullptr), m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	for (auto _mapIter = m_mapAnimation.begin(); _mapIter != m_mapAnimation.end(); ++_mapIter)
		if(_mapIter->second != nullptr)
			delete _mapIter->second;
	m_mapAnimation.clear();
}

void CAnimator::update()
{
	if (m_pCurAnimation != nullptr) {
		m_pCurAnimation->update();

		if (m_bRepeat && m_pCurAnimation->IsFinish()) {
			m_pCurAnimation->SetFrame(0);
		}
	}
		
}

void CAnimator::render(HDC _dc)
{
	if (m_pCurAnimation != nullptr)
		m_pCurAnimation->render(_dc);
}

void CAnimator::CreateAnimation(const std::wstring& _strName, CTexture* _pTexture, const Vec2& _vLT, const Vec2& _vFrameSize, const Vec2& _vStep, float _fDuration, const Vec2& _vOffset, UINT _iFrameCount)
{
	CAnimation* pAnimation = FindAnimation(_strName);
	assert(pAnimation == nullptr);

	pAnimation = new CAnimation;

	pAnimation->SetName(_strName);
	pAnimation->SetAnimator(this);
	pAnimation->Create(_pTexture, _vLT, _vFrameSize, _vStep, _fDuration, _vOffset, _iFrameCount);	// 애니메이션 정보를 넣어서 생성

	m_mapAnimation.insert(std::make_pair(_strName, pAnimation));
}

void CAnimator::CreateAnimation(const std::wstring& _strName, CTexture* _pTexture, const Vec2& _vLT, const Vec2& _vFrameSize, const Vec2& _vStep, float _fDuration, const Vec2& _vOffset, const Vec2& _vScale, UINT _iFrameCount)
{
	CAnimation* pAnimation = FindAnimation(_strName);
	assert(pAnimation == nullptr);

	pAnimation = new CAnimation;

	pAnimation->SetName(_strName);
	pAnimation->SetAnimator(this);
	pAnimation->SetScale(_vScale);
	pAnimation->Create(_pTexture, _vLT, _vFrameSize, _vStep, _fDuration, _vOffset, _iFrameCount);	// 애니메이션 정보를 넣어서 생성

	m_mapAnimation.insert(std::make_pair(_strName, pAnimation));
}

CAnimation* CAnimator::FindAnimation(const std::wstring& _strName)
{
	auto iter = m_mapAnimation.find(_strName);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}

void CAnimator::Play(const std::wstring& _strName, bool _bRepeat)
{
	if (m_pCurAnimation != FindAnimation(_strName)) {
		m_pCurAnimation = FindAnimation(_strName);
		m_bRepeat = _bRepeat;
	}
}
