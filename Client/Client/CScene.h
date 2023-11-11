#pragma once

// 컴파일 속도 향상을 위한 선언 전방 참조
// 헤더가 서로를 참조하면, 코드 수정 시 컴파일러는 모든 코드를 다 컴파일 해야한다.
// 선언 전방 참조를 하면, 참조된 클래스가 변경되어도 참조한 클래스는 컴파일 하지 않는다.
// 하지만 클래스에 대한 구체적인 정보가 없기 때문에, 멤버 변수나 멤버 함수는 직접 사용할 수 없다.(포인터 or 레퍼런스를 사용해야함)
class CObject;

class CScene
{
private:
	// 부모 포인터 타입으로 자식 포인터 타입을 모두 받을 수 있다.
	// 따라서 그룹 개수 만큼의 vector<CObject*>로 모든 Object들을 관리한다.

	std::array<std::vector<CObject*>, (UINT)GROUP_TYPE::END>	m_arrVecObj;	// Object들을 관리할 vector array
	std::wstring												m_strName;		// Scene 이름

public:
	const std::wstring& GetName()									{ return m_strName; }
	const std::vector<CObject*>& GetGroupObject(GROUP_TYPE _eType)	{ return m_arrVecObj[(UINT)_eType]; }

	void SetName(const std::wstring& _strName)						{ m_strName = _strName; }

	virtual void update();
	void finalUpdate();		// 최종 업데이트 (확정짓는 역할, 작업의 마무리)
	void render(HDC _dc);

	// 앞으로 CScene에서 파생되는 모든 클래스들은 이 함수를 구현해야함. (순수 가상 함수, 인터페이스화)
	// 순수 가상 함수는 함수 선언의 끝에 = 0으로 표시. 순수 가상 함수는 구현을 가지지 않으며, 
	// 파생 클래스에서 이를 반드시 재정의해야 함. 
	// 파생 클래스에서 순수 가상 함수를 재정의하지 않으면 해당 클래스도 추상 클래스가 되며, 이를 인스턴스화할 수 없음.
	virtual void Enter()	= 0;	// Scene 진입
	virtual void Exit()		= 0;	// Scene 탈출

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)	{ m_arrVecObj[(UINT)_eType].push_back(_pObj); }
	CObject* FindObject(std::wstring _strName);
	void DeleteGroup(GROUP_TYPE _eType);
	void DeleteAll();

public:
	CScene();
	virtual ~CScene();
};

