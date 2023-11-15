#pragma once
// Core Class: 프로그램 그 자체를 나타내는 객체를 표현할 클래스
// 
// 싱글톤 패턴: 객체의 생성을 1개로 제한한다.
// 언제 어디서든 쉽게 접근 가능해야한다.
// 
// 생성자를 private로 둬서, 마음대로 생성되지 않도록 하고,
// 정해진 함수를 통해서만 생성자를 호출할 수 있도록 한다. -> 객체의 생성을 제한할 수 있음.

// 싱글톤 패턴 1

//class CCore
//{
//private:
//	static CCore* g_pInst;
//
//public:
//	// static keyword를 사용해서, 객체가 있든 없든 사용할 수 있는 정적 멤버 함수를 만든다.
//	// 객체 없이 호출이 가능하므로, 멤버 변수에 접근할 수 없고(정적 멤버 제외), this도 없다.
//
//	// 정적 멤버 변수는 데이터 영역에 생성되고, 해당 클래스와 해당 클래스의 정적 멤버 함수 내에서 접근할 수 있다.
//	// 정적 멤버 변수는 반드시 클래스 외부에서 초기화를 해주어야 한다.
//	static CCore* GetInstance() 
//	{ 
//		// 최초 호출
//		if (g_pInst == nullptr) { g_pInst = new CCore; }
//		
//		return g_pInst;
//	}
//
//	static void Release() 
//	{
//		if (g_pInst != nullptr) 
//		{ 
//			delete g_pInst;
//			g_pInst = nullptr;
//		}
//	}
//
//private:
//	CCore() {}
//	~CCore() {}
//};

#include "define.h"

// 싱글톤 패턴 2
// 이런식으로 싱글톤 패턴을 구현할 수도 있다.
// 코어의 해제를 신경쓰지 않아도 된다. 알아서 다 쓰면 해제해줌.
class CCore
{
	// GetInst 함수로 객체를 하나만 생성하는 싱글톤 패턴 방식을 많이 사용할 것 이기 때문에, 
	// 매크로 함수로 만들어 둘 것임. (define.h)
	SINGLE(CCore)
private:
	HWND		m_hWnd;			// 메인 윈도우 핸들
	POINT		m_ptResolution;	// 메인 윈도우 해상도
	HDC			m_hDC;			// 메인 윈도우 Device Context

	HBITMAP		m_hBit;			// 후면	버퍼 비트맵 핸들
	HDC			m_memDC;		// 후면 버퍼 Device Context (memory DC)

	std::array<HBRUSH, (UINT)BRUSH_TYPE::END>	m_arrBrush;		// 자주 사용할 브러쉬의 배열
	std::array<HPEN, (UINT)PEN_TYPE::END>		m_arrPen;		// 자주 사용할 펜의 배열

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void CreateBrushPen();
	void progress();

public:
	HWND GetMainHwnd()					{ return m_hWnd; }
	HDC GetMainDC()						{ return m_hDC; }
	POINT GetResolution()				{ return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType)	{ return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType)		{ return m_arrPen[(UINT)_eType]; }
};
