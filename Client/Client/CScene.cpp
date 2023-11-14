#include "pch.h"
#include "CScene.h"
#include "CObject.h"

CScene::CScene() : m_arrVecObj{}, m_strName{}
{

}

CScene::~CScene()
{
	// Scene Objects ����
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj) {
			delete _pObj;
		}
	}
}

void CScene::update()
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj) {
			if (!_pObj->IsDead())
				_pObj->update();
		}
	}
}

void CScene::finalUpdate()
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj) {
			_pObj->finalUpdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {

		auto pObjIter = _vecObj.begin();

		while (pObjIter != _vecObj.end())
		{
			// ���� ������ �ƴ� ������Ʈ�� �׸���.
			if (!(*pObjIter)->IsDead()) {
				(*pObjIter)->render(_dc);
				++pObjIter;
			}
			else {
				// ���� ������ ������Ʈ�� ���� ������ ����
				pObjIter = _vecObj.erase(pObjIter);
			}
		}
	}
}

CObject* CScene::FindObject(std::wstring _strName)
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj) {
			if (_pObj->GetName() == _strName)
				return _pObj;
		}
	}

	return nullptr;
}

void CScene::DeleteGroup(GROUP_TYPE _eType)
{
	for (CObject* _pObj : m_arrVecObj[(UINT)_eType])
		delete _pObj;

	m_arrVecObj[(UINT)_eType].clear();
}

void CScene::DeleteAll()
{
	for (std::vector<CObject*>& _vecObj : m_arrVecObj) {
		for (CObject* _pObj : _vecObj)
			delete _pObj;

		_vecObj.clear();
	}
}
