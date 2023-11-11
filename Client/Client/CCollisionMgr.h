#pragma once

class CCollider;

// ID를 담을 유니온
union COLLIDER_ID
{
	struct {
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr)
private:
	std::map<ULONGLONG, bool>					m_mapColInfo;	// 충돌체 간의 이전 프레임 충돌 정보
	std::array<UINT, (UINT)GROUP_TYPE::END>		m_arrCheck;		// 충돌 체크 행렬 (32bit x 32)

public:
	void update();
	void CheckGroup(GROUP_TYPE _eLHS, GROUP_TYPE _eRHS);
	void Reset() { m_arrCheck.fill(0); }

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLHS, GROUP_TYPE _eRHS);
	bool IsCollision(CCollider* _pLHS, CCollider* _pRHS);
};

