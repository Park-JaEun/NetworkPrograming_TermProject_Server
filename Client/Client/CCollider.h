#pragma once

class CObject;

class CCollider
{
private:
	static UINT g_iNextID;

	CObject*	m_pObject;		// 오브젝트

	Vec2		m_vOffsetPos;	// 오브젝트로 부터 상대적인 위치
	Vec2		m_vFinalPos;	// finalUpdate에서 메 프레임마다 갱신
	Vec2		m_vScale;		// 충돌체 크기

	UINT		m_iID;			// 충돌체 고유한 ID 값
	
	int			m_iCol;			// 충돌 여부

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
	void OnCollision(CCollider* _pOther);		// 충돌 중인 경우 호출되는 함수
	void EnterCollision(CCollider* _pOther);	// 막 충돌이 일어난 경우 호출되는 함수
	void ExitCollision(CCollider* _pOther);		// 막 충돌에서 벗어난 경우 호출되는 함수

public:
	CCollider();
	CCollider(const CCollider& _origin);	// 복사 생성 되더라도 ID 값을 바꿔줘야하기 때문에, 복사 생성자 정의
	CCollider& operator= (CCollider& _origin) = delete;	// = 연산자는 안 쓸 것이므로 삭제하여 대입과 복사할당을 막음

	~CCollider();
};