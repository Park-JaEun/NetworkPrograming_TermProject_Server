#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CObject.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr() : m_arrCheck{}
{
	
}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::update()
{
	// 체크된 그룹들은 충돌체크를 해야한다고 업데이트
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow) {
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol) {
			if (m_arrCheck[iRow] & (1 << iCol)) {
				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLHS, GROUP_TYPE _eRHS)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	const std::vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLHS);
	const std::vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRHS);

	for (CObject* pObjLeft : vecLeft) {
		if (pObjLeft->GetCollider() == nullptr)	// 충돌체가 없을 경우 continue
			continue;

		for (CObject* pObjRight : vecRight) {
			if (pObjRight->GetCollider() == nullptr || vecLeft == vecRight) // 충돌체가 없거나 양쪽 obj가 모두 자신일 경우 continue
				continue;

			CCollider* pLeftCol = pObjLeft->GetCollider();
			CCollider* pRightCol = pObjRight->GetCollider();

			// ID 값은 두 ID(Left, Right)의 조합으로 결정됨.
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			auto iter = m_mapColInfo.find(ID.ID);					// 충돌 정보 받아옴

			if (iter == m_mapColInfo.end()) {
				m_mapColInfo.insert(std::make_pair(ID.ID, false));	// 최초 등록, 이전엔 충돌 X
				iter = m_mapColInfo.find(ID.ID);					// 충돌 정보 받아옴
			}

			if (IsCollision(pLeftCol, pRightCol)) {
				// 현재 충돌
				if (iter->second) {
					// 이전 프레임 충돌 O
					if (pObjLeft->IsDead() || pObjRight->IsDead()) {
						// 둘 중 하나가 삭제 예정이면 충돌 해제
						pLeftCol->ExitCollision(pRightCol);
						pRightCol->ExitCollision(pLeftCol);
						iter->second = false;
					}
					else {
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}
				else {
					if (!pObjLeft->IsDead() && !pObjRight->IsDead()) {
						// 이전 프레임 충돌 X
						// 둘 다 삭제 예정이 아니면 충돌
						pLeftCol->EnterCollision(pRightCol);
						pRightCol->EnterCollision(pLeftCol);
						iter->second = true;
					}
				}
			}
			else {
				// 현재 충돌 X
				if (iter->second) {
					// 이전 프레임 충돌 O
					pLeftCol->ExitCollision(pRightCol);
					pRightCol->ExitCollision(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLHS, CCollider* _pRHS)
{
	RECT tempRECT{};

	Vec2 vLeftPos	= _pLHS->GetFinalPos();
	Vec2 vLeftScale = _pLHS->GetScale();
	RECT rectLeft{
		(int)(vLeftPos.x - vLeftScale.x / 2.f),
		(int)(vLeftPos.y - vLeftScale.y / 2.f),
		(int)(vLeftPos.x + vLeftScale.x / 2.f),
		(int)(vLeftPos.y + vLeftScale.y / 2.f) };

	Vec2 vRightPos = _pRHS->GetFinalPos();
	Vec2 vRightScale = _pRHS->GetScale();
	RECT rectRight{
		(int)(vRightPos.x - vRightScale.x / 2.f),
		(int)(vRightPos.y - vRightScale.y / 2.f),
		(int)(vRightPos.x + vRightScale.x / 2.f),
		(int)(vRightPos.y + vRightScale.y / 2.f) };

	// 충돌검사
	if (IntersectRect(&tempRECT, &rectLeft, &rectRight))
		return true;
	return false;
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLHS, GROUP_TYPE _eRHS)
{
	// 더 작은 값의 그룹 타입을 행으로, 큰 값을 열으로 사용하여 비트연산
	UINT iRow = (UINT)_eLHS;
	UINT iCol = (UINT)_eRHS;
	
	if (iRow > iCol) {
		iRow = (UINT)_eRHS;
		iCol = (UINT)_eLHS;
	}

	if (m_arrCheck[iRow] & (1 << iCol)) {
		m_arrCheck[iRow] &= ~(1 << iCol);	// 체크 해제
	}
	else {
		m_arrCheck[iRow] |= (1 << iCol);	// 체크
	}
}