#pragma once

class CObject;

class CCamera
{
	SINGLE(CCamera)
private:
	Vec2		m_vLookAt;		// 카메라가 보는 위치
	Vec2		m_vPrevLookAt;		// 카메라의 이전 프레임 위치
	CObject*	m_pTargetObj;	// 카메라 타겟 오브젝트
	Vec2		m_vDiff;		// 화면 중앙 좌표와 카메라 LookAt 좌표간의 차이

public:
	Vec2 GetRenderPos(Vec2 _vPos)		{ return _vPos - m_vDiff; }
	Vec2 GetLookAt()					{ return m_vLookAt; }
	CObject* GetTarget()				{ return m_pTargetObj; }

	void SetLookAt(const Vec2& _vLook)	{ m_vLookAt = _vLook; }
	void SetTarget(CObject* _pTarget)	{ m_pTargetObj = _pTarget; }

public:
	void update();

private:
	void CalDiff();	// 화면 중앙 좌표와 카메라 LookAt 좌표의 차이 계산

};

