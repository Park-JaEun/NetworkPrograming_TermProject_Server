#pragma once

class CObject;

class CCollider
{
private:
	static UINT g_iNextID;

	CObject*	m_pObject;		// ������Ʈ

	Vec2		m_vOffsetPos;	// ������Ʈ�� ���� ������� ��ġ
	Vec2		m_vFinalPos;	// finalUpdate���� �� �����Ӹ��� ����
	Vec2		m_vScale;		// �浹ü ũ��

	UINT		m_iID;			// �浹ü ������ ID ��
	
	int			m_iCol;			// �浹 ����

public:
	void SetObj(CObject* _pObject)			{ m_pObject = _pObject; }
	void SetOffsetPos(const Vec2& _vPos)	{ m_vOffsetPos = _vPos; }
	void SetScale(const Vec2& _vScale)		{ m_vScale = _vScale; }

	const Vec2 GetOffsetPos()				{ return m_vOffsetPos; }
	const Vec2 GetFinalPos()				{ return m_vFinalPos; }
	const Vec2 GetScale()					{ return m_vScale; }
	const UINT GetID()						{ return m_iID; }
	CObject* GetObj()						{ return m_pObject; }

public:
	void finalUpdate();
	void render(HDC _dc);

public:
	bool PtInCollider(Vec2 vPos);
	void OnCollision(CCollider* _pOther);		// �浹 ���� ��� ȣ��Ǵ� �Լ�
	void EnterCollision(CCollider* _pOther);	// �� �浹�� �Ͼ ��� ȣ��Ǵ� �Լ�
	void ExitCollision(CCollider* _pOther);		// �� �浹���� ��� ��� ȣ��Ǵ� �Լ�

public:
	CCollider();
	CCollider(const CCollider& _origin);	// ���� ���� �Ǵ��� ID ���� �ٲ�����ϱ� ������, ���� ������ ����
	CCollider& operator= (CCollider& _origin) = delete;	// = �����ڴ� �� �� ���̹Ƿ� �����Ͽ� ���԰� �����Ҵ��� ����

	~CCollider();
};