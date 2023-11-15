#include "pch.h"

#include "CAnimation.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"

#include "CTimer.h"
#include "CCamera.h"

CAnimation::CAnimation() 
	: m_strName{}
	, m_pAnimator(nullptr)
	, m_pTexture(nullptr)
	, m_vecFrame{}
	, m_iCurFrame(0)
	, m_fAccTime(0.f)
	, m_vScale(1.f, 1.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::update()
{
	if (m_bFinish)	// 재생이 끝났을 경우 업데이트 하지 않는다.
		return;

	m_fAccTime += DT;

	if (m_fAccTime > m_vecFrame[m_iCurFrame].fDuration) {	// 현재 프레임 지연시간이 지나면
		m_fAccTime = m_fAccTime - m_vecFrame[m_iCurFrame].fDuration;	// 누적 시간 변경

		++m_iCurFrame;	// 프레임 증가

		if (m_iCurFrame >= m_vecFrame.size())	// 프레임이 끝나면
		{
			m_iCurFrame = -1;
			m_bFinish = true;
		}
	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bFinish)	// 재생이 끝났을 경우 그리지 않는다.
		return;

	CObject* pObject = m_pAnimator->GetObj();
	Vec2 vPos = pObject->GetPos();

	// 상대 위치 갱신
	vPos.x += m_vecFrame[m_iCurFrame].vOffset.x;
	vPos.y += m_vecFrame[m_iCurFrame].vOffset.y;

	// 렌더링 좌표로 변환
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vPos);

	TransparentBlt(_dc,
		(int)(vRenderPos.x - m_vecFrame[m_iCurFrame].vFrameSize.x * m_vScale.x / 2.f),
		(int)(vRenderPos.y - m_vecFrame[m_iCurFrame].vFrameSize.y * m_vScale.y / 2.f),
		(int)m_vecFrame[m_iCurFrame].vFrameSize.x * m_vScale.x,
		(int)m_vecFrame[m_iCurFrame].vFrameSize.y * m_vScale.y,
		m_pTexture->GetDC(),
		(int)m_vecFrame[m_iCurFrame].vLT.x,
		(int)m_vecFrame[m_iCurFrame].vLT.y,
		(int)m_vecFrame[m_iCurFrame].vFrameSize.x,
		(int)m_vecFrame[m_iCurFrame].vFrameSize.y,
		RGB(255, 0, 255));

	/*TransparentBlt(_dc,
		(int)(vRenderPos.x - pObject->GetScale().x * m_vScale.x / 2.f),
		(int)(vRenderPos.y - pObject->GetScale().y * m_vScale.y / 2.f),
		(int)pObject->GetScale().x * m_vScale.x,
		(int)pObject->GetScale().y * m_vScale.y,
		m_pTexture->GetDC(),
		(int)m_vecFrame[m_iCurFrame].vLT.x,
		(int)m_vecFrame[m_iCurFrame].vLT.y,
		(int)m_vecFrame[m_iCurFrame].vFrameSize.x,
		(int)m_vecFrame[m_iCurFrame].vFrameSize.y,
		RGB(255, 0, 255));*/
}

void CAnimation::Create(CTexture* _pTexture, const Vec2& _vLT, const Vec2& _vFrameSize, const Vec2& _vStep, float _fDuration, const Vec2& _vOffset, UINT _iFrameCount)
{
	m_pTexture = _pTexture;

	tAnimationFrame frame = {};
	for (UINT i = 0; i < _iFrameCount; ++i) {
		frame.fDuration = _fDuration;
		frame.vFrameSize = _vFrameSize;
		frame.vLT = _vLT + _vStep * i;
		frame.vOffset = _vOffset;

		m_vecFrame.push_back(frame);
	}

}
