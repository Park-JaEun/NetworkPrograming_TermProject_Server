#pragma once

class CCollider;

class CObject {
private:
	std::wstring	m_strName;		// 이름

	Vec2			m_vPos;			// 좌표
	Vec2			m_vScale;		// 스케일

	CCollider* m_pCollider;			// 충돌체

	bool m_bAlive;					// Dead? Alive?

public:
	const Vec2 GetPos()				{ return m_vPos; }
	const Vec2 GetScale()			{ return m_vScale; }
	CCollider* GetCollider()		{ return m_pCollider; }
	const std::wstring& GetName()	{ return m_strName; }
	const bool IsDead()				{ return !m_bAlive; }

	void SetPos(const Vec2& _vPos)				{ m_vPos = _vPos; }
	void SetScale(const Vec2& _vScale)			{ m_vScale = _vScale; }
	void SetName(const std::wstring& _strName)	{ m_strName = _strName; }

	void CreateCollider();

private:
	void SetDead() { m_bAlive = false; }

public:
	virtual void update() = 0;
	virtual void finalUpdate() final; // 함수를 상속하지 않고, 오버라이딩을 막기 위해 final 키워드를 붙인다. 이 함수가 최종이라는 뜻임

	virtual void OnCollision(CCollider* _pOther) {}		// 충돌 중인 경우 호출되는 함수
	virtual void EnterCollision(CCollider* _pOther) {}	// 막 충돌이 일어난 경우 호출되는 함수
	virtual void ExitCollision(CCollider* _pOther) {}	// 막 충돌에서 벗어난 경우 호출되는 함수

	virtual CObject* Clone() = 0;	// 자신의 복사본을 반환하는 함수

public:
	CObject();
	CObject(const CObject& _origin);	// 복사 생성자
	virtual ~CObject();

	friend class CEventMgr;
};

