#pragma once

class CObject;

class CCamera
{
	SINGLE(CCamera)
private:
	Vec2		m_vLookAt;		// ī�޶� ���� ��ġ
	Vec2		m_vPrevLookAt;		// ī�޶��� ���� ������ ��ġ
	CObject*	m_pTargetObj;	// ī�޶� Ÿ�� ������Ʈ
	Vec2		m_vDiff;		// ȭ�� �߾� ��ǥ�� ī�޶� LookAt ��ǥ���� ����

public:
	Vec2 GetRenderPos(Vec2 _vPos)		{ return _vPos - m_vDiff; }
	Vec2 GetLookAt()					{ return m_vLookAt; }
	CObject* GetTarget()				{ return m_pTargetObj; }

	void SetLookAt(const Vec2& _vLook)	{ m_vLookAt = _vLook; }
	void SetTarget(CObject* _pTarget)	{ m_pTargetObj = _pTarget; }

public:
	void update();

private:
	void CalDiff();	// ȭ�� �߾� ��ǥ�� ī�޶� LookAt ��ǥ�� ���� ���

};

