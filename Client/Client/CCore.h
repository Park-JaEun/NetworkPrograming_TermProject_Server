#pragma once
#include "define.h"

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
	SOCKET		m_sock;			// ������ ������ ����

	std::wstring m_strNickName;	// �г���
	int 		m_iID;			// ID�� (1p, 2p, 3p)

	std::array<HBRUSH, (UINT)BRUSH_TYPE::END>	m_arrBrush;		// ���� ����� �귯���� �迭
	std::array<HPEN, (UINT)PEN_TYPE::END>		m_arrPen;		// ���� ����� ���� �迭

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void CreateBrushPen();
	void CommunicationToServer();
	void progress();

public:
	HWND GetMainHwnd()					{ return m_hWnd; }
	HDC GetMainDC()						{ return m_hDC; }
	POINT GetResolution()				{ return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType)	{ return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType)		{ return m_arrPen[(UINT)_eType]; }
	SOCKET GetSocket()					{ return m_sock; }
	std::wstring GetNickName()			{ return m_strNickName; }
	int GetID()							{ return m_iID; }

	void SetSocket(SOCKET _sock)		{ m_sock = _sock; }
	void SetNickName(std::wstring _str)	{ m_strNickName = _str; }
	void SetID(int _iID)				{ m_iID = _iID; }
};
