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
	// üũ�� �׷���� �浹üũ�� �ؾ��Ѵٰ� ������Ʈ
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
		if (pObjLeft->GetCollider() == nullptr)	// �浹ü�� ���� ��� continue
			continue;

		for (CObject* pObjRight : vecRight) {
			if (pObjRight->GetCollider() == nullptr || vecLeft == vecRight) // �浹ü�� ���ų� ���� obj�� ��� �ڽ��� ��� continue
				continue;

			CCollider* pLeftCol = pObjLeft->GetCollider();
			CCollider* pRightCol = pObjRight->GetCollider();

			// ID ���� �� ID(Left, Right)�� �������� ������.
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			auto iter = m_mapColInfo.find(ID.ID);					// �浹 ���� �޾ƿ�

			if (iter == m_mapColInfo.end()) {
				m_mapColInfo.insert(std::make_pair(ID.ID, false));	// ���� ���, ������ �浹 X
				iter = m_mapColInfo.find(ID.ID);					// �浹 ���� �޾ƿ�
			}

			if (IsCollision(pLeftCol, pRightCol)) {
				// ���� �浹
				if (iter->second) {
					// ���� ������ �浹 O
					if (pObjLeft->IsDead() || pObjRight->IsDead()) {
						// �� �� �ϳ��� ���� �����̸� �浹 ����
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
						// ���� ������ �浹 X
						// �� �� ���� ������ �ƴϸ� �浹
						pLeftCol->EnterCollision(pRightCol);
						pRightCol->EnterCollision(pLeftCol);
						iter->second = true;
					}
				}
			}
			else {
				// ���� �浹 X
				if (iter->second) {
					// ���� ������ �浹 O
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

	// �浹�˻�
	if (IntersectRect(&tempRECT, &rectLeft, &rectRight))
		return true;
	return false;
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLHS, GROUP_TYPE _eRHS)
{
	// �� ���� ���� �׷� Ÿ���� ������, ū ���� ������ ����Ͽ� ��Ʈ����
	UINT iRow = (UINT)_eLHS;
	UINT iCol = (UINT)_eRHS;
	
	if (iRow > iCol) {
		iRow = (UINT)_eRHS;
		iCol = (UINT)_eLHS;
	}

	if (m_arrCheck[iRow] & (1 << iCol)) {
		m_arrCheck[iRow] &= ~(1 << iCol);	// üũ ����
	}
	else {
		m_arrCheck[iRow] |= (1 << iCol);	// üũ
	}
}