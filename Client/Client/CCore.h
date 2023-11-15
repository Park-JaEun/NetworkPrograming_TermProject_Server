#pragma once
// Core Class: ���α׷� �� ��ü�� ��Ÿ���� ��ü�� ǥ���� Ŭ����
// 
// �̱��� ����: ��ü�� ������ 1���� �����Ѵ�.
// ���� ��𼭵� ���� ���� �����ؾ��Ѵ�.
// 
// �����ڸ� private�� �ּ�, ������� �������� �ʵ��� �ϰ�,
// ������ �Լ��� ���ؼ��� �����ڸ� ȣ���� �� �ֵ��� �Ѵ�. -> ��ü�� ������ ������ �� ����.

// �̱��� ���� 1

//class CCore
//{
//private:
//	static CCore* g_pInst;
//
//public:
//	// static keyword�� ����ؼ�, ��ü�� �ֵ� ���� ����� �� �ִ� ���� ��� �Լ��� �����.
//	// ��ü ���� ȣ���� �����ϹǷ�, ��� ������ ������ �� ����(���� ��� ����), this�� ����.
//
//	// ���� ��� ������ ������ ������ �����ǰ�, �ش� Ŭ������ �ش� Ŭ������ ���� ��� �Լ� ������ ������ �� �ִ�.
//	// ���� ��� ������ �ݵ�� Ŭ���� �ܺο��� �ʱ�ȭ�� ���־�� �Ѵ�.
//	static CCore* GetInstance() 
//	{ 
//		// ���� ȣ��
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

// �̱��� ���� 2
// �̷������� �̱��� ������ ������ ���� �ִ�.
// �ھ��� ������ �Ű澲�� �ʾƵ� �ȴ�. �˾Ƽ� �� ���� ��������.
class CCore
{
	// GetInst �Լ��� ��ü�� �ϳ��� �����ϴ� �̱��� ���� ����� ���� ����� �� �̱� ������, 
	// ��ũ�� �Լ��� ����� �� ����. (define.h)
	SINGLE(CCore)
private:
	HWND		m_hWnd;			// ���� ������ �ڵ�
	POINT		m_ptResolution;	// ���� ������ �ػ�
	HDC			m_hDC;			// ���� ������ Device Context

	HBITMAP		m_hBit;			// �ĸ�	���� ��Ʈ�� �ڵ�
	HDC			m_memDC;		// �ĸ� ���� Device Context (memory DC)

	std::array<HBRUSH, (UINT)BRUSH_TYPE::END>	m_arrBrush;		// ���� ����� �귯���� �迭
	std::array<HPEN, (UINT)PEN_TYPE::END>		m_arrPen;		// ���� ����� ���� �迭

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
