#include "pch.h"
#include "CObjectMgr.h"
#include "CObject.h"

CObjectMgr::CObjectMgr()
{
}

CObjectMgr::~CObjectMgr()
{
	DeleteAll();
}

void CObjectMgr::update()
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj) {
			if (!_pObj->IsDead()) {
				_pObj->update();
				_pObj->finalUpdate();
			}
		}
	}
}

void CObjectMgr::finalUpdate()
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj) {
			_pObj->finalUpdate();
		}
	}
}

void CObjectMgr::DeleteDeadObject()
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {

		auto pObjIter = _vecObj.begin();

		while (pObjIter != _vecObj.end()) {
			if ((*pObjIter)->IsDead())
				// 삭제 예정인 오브젝트는 벡터 내에서 삭제
				pObjIter = _vecObj.erase(pObjIter);
		}
	}
}

CObject* CObjectMgr::FindObject(std::wstring _strName)
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj) {
			if (_pObj->GetName() == _strName)
				return _pObj;
		}
	}

	return nullptr;
}

void CObjectMgr::DeleteGroup(GROUP_TYPE _eType)
{
	for (CObject* _pObj : m_arrVecObj[(UINT)_eType])
		delete _pObj;

	m_arrVecObj[(UINT)_eType].clear();
}

void CObjectMgr::DeleteAll()
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj)
			delete _pObj;

		_vecObj.clear();
	}
}
