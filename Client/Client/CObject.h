#pragma once

#include "CCamera.h"

class CCollider;
class CAnimator;

class CObject
{
private:
	std::wstring	m_strName;		// �̸�

	Vec2			m_vPos;			// ��ǥ
	Vec2			m_vScale;		// ������

	// �浹 ������ �ʿ��ϴ�. ��� �ұ�?
	// -> �浹�� �ʿ��� ������Ʈ���� ����� ���� ���� 
	// -> ������ Ȯ�强�� ������. �浹�� �ʿ����� ���� �ְ� �浹�� �ʿ������� ��� ����? �� �ٲ���� ����? 
	// -> ������Ʈ ���� ����� ���α׷��� ���
	CCollider*		m_pCollider;	// �浹ü
	CAnimator*		m_pAnimator;	// �ִϸ�����

	bool m_bAlive;					// Dead? Alive?

public:
	const Vec2 GetPos()							{ return m_vPos; }
	const Vec2 GetScale()						{ return m_vScale; }
	CCollider* GetCollider()					{ return m_pCollider; }
	CAnimator* GetAnimator()					{ return m_pAnimator; }
	const std::wstring& GetName()				{ return m_strName; }
	const bool IsDead()							{ return !m_bAlive; }

	void SetPos(const Vec2& _vPos)				{ m_vPos = _vPos; }
	void SetScale(const Vec2& _vScale)			{ m_vScale = _vScale; }
	void SetAnimator(CAnimator* _pAnimator)		{ m_pAnimator = _pAnimator; }
	void SetName(const std::wstring& _strName)	{ m_strName = _strName; }

	void CreateCollider();
	virtual void CreateAnimator() {}

private:
	void SetDead()	{ m_bAlive = false; }

public:
	virtual void update() = 0;
	virtual void finalUpdate() final; // �Լ��� ������� �ʰ�, �������̵��� ���� ���� final Ű���带 ���δ�. �� �Լ��� �����̶�� ����
	virtual void render(HDC _dc);
	void componentRender(HDC _dc);

	virtual void OnCollision(CCollider* _pOther) {}		// �浹 ���� ��� ȣ��Ǵ� �Լ�
	virtual void EnterCollision(CCollider* _pOther) {}	// �� �浹�� �Ͼ ��� ȣ��Ǵ� �Լ�
	virtual void ExitCollision(CCollider* _pOther) {}	// �� �浹���� ��� ��� ȣ��Ǵ� �Լ�

	virtual CObject* Clone() = 0;	// �ڽ��� ���纻�� ��ȯ�ϴ� �Լ�

public:
	CObject();
	CObject(const CObject& _origin);	// ���� ������
	virtual ~CObject();

	friend class CEventMgr;
};

