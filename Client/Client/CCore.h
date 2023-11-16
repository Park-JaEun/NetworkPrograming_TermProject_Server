#pragma once
#include "define.h"

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
	SOCKET		m_sock;			// 서버와 연결할 소켓

	std::wstring m_strNickName;	// 닉네임
	int 		m_iID;			// ID값 (1p, 2p, 3p)

	std::array<HBRUSH, (UINT)BRUSH_TYPE::END>	m_arrBrush;		// 자주 사용할 브러쉬의 배열
	std::array<HPEN, (UINT)PEN_TYPE::END>		m_arrPen;		// 자주 사용할 펜의 배열

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
