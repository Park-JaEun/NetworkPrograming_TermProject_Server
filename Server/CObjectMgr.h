#pragma once

class CObject;

class CObjectMgr {
	SINGLE(CObjectMgr)
private:
	// 부모 포인터 타입으로 자식 포인터 타입을 모두 받을 수 있다.
	// 따라서 그룹 개수 만큼의 vector<CObject*>로 모든 Object들을 관리한다.
	std::array<std::vector<CObject*>, (UINT)GROUP_TYPE::END>	m_arrVecObj;	// Object들을 관리할 vector array

public:
	const std::vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrVecObj[(UINT)_eType]; }
	CObject* FindObject(std::wstring _strName);

	virtual void update();
	void finalUpdate();		// 최종 업데이트 (확정짓는 역할, 작업의 마무리)

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrVecObj[(UINT)_eType].push_back(_pObj); }
	void DeleteDeadObject();
	void DeleteGroup(GROUP_TYPE _eType);
	void DeleteAll();
};

