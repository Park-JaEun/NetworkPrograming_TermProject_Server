#pragma once

// ������ �ӵ� ����� ���� ���� ���� ����
// ����� ���θ� �����ϸ�, �ڵ� ���� �� �����Ϸ��� ��� �ڵ带 �� ������ �ؾ��Ѵ�.
// ���� ���� ������ �ϸ�, ������ Ŭ������ ����Ǿ ������ Ŭ������ ������ ���� �ʴ´�.
// ������ Ŭ������ ���� ��ü���� ������ ���� ������, ��� ������ ��� �Լ��� ���� ����� �� ����.(������ or ���۷����� ����ؾ���)
class CObject;

class CScene
{
private:
	// �θ� ������ Ÿ������ �ڽ� ������ Ÿ���� ��� ���� �� �ִ�.
	// ���� �׷� ���� ��ŭ�� vector<CObject*>�� ��� Object���� �����Ѵ�.

	std::array<std::vector<CObject*>, (UINT)GROUP_TYPE::END>	m_arrVecObj;	// Object���� ������ vector array
	std::wstring												m_strName;		// Scene �̸�

public:
	const std::wstring& GetName()									{ return m_strName; }
	const std::vector<CObject*>& GetGroupObject(GROUP_TYPE _eType)	{ return m_arrVecObj[(UINT)_eType]; }

	void SetName(const std::wstring& _strName)						{ m_strName = _strName; }

	virtual void update();
	void finalUpdate();		// ���� ������Ʈ (Ȯ������ ����, �۾��� ������)
	void render(HDC _dc);

	// ������ CScene���� �Ļ��Ǵ� ��� Ŭ�������� �� �Լ��� �����ؾ���. (���� ���� �Լ�, �������̽�ȭ)
	// ���� ���� �Լ��� �Լ� ������ ���� = 0���� ǥ��. ���� ���� �Լ��� ������ ������ ������, 
	// �Ļ� Ŭ�������� �̸� �ݵ�� �������ؾ� ��. 
	// �Ļ� Ŭ�������� ���� ���� �Լ��� ���������� ������ �ش� Ŭ������ �߻� Ŭ������ �Ǹ�, �̸� �ν��Ͻ�ȭ�� �� ����.
	virtual void Enter()	= 0;	// Scene ����
	virtual void Exit()		= 0;	// Scene Ż��

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)	{ m_arrVecObj[(UINT)_eType].push_back(_pObj); }
	CObject* FindObject(std::wstring _strName);
	void DeleteGroup(GROUP_TYPE _eType);
	void DeleteAll();

public:
	CScene();
	virtual ~CScene();
};

