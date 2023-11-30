#pragma once

class CObject;

class CObjectMgr {
	SINGLE(CObjectMgr)
private:
	// �θ� ������ Ÿ������ �ڽ� ������ Ÿ���� ��� ���� �� �ִ�.
	// ���� �׷� ���� ��ŭ�� vector<CObject*>�� ��� Object���� �����Ѵ�.
	std::array<std::vector<CObject*>, (UINT)GROUP_TYPE::END>	m_arrVecObj;	// Object���� ������ vector array

public:
	const std::vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrVecObj[(UINT)_eType]; }
	CObject* FindObject(std::wstring _strName);

	virtual void update();

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrVecObj[(UINT)_eType].push_back(_pObj); }
	void DeleteDeadObject();
	void DeleteGroup(GROUP_TYPE _eType);
	void DeleteAll();
};

