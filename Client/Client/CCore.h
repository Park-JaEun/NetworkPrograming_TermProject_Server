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
	bool        m_bIsStart;     // ���� ���� ����
	bool		m_bIsGameClear;
	bool		m_bIsGameOver;

	std::array<HBRUSH, (UINT)BRUSH_TYPE::END>	m_arrBrush;		// ���� ����� �귯���� �迭
	std::array<HPEN, (UINT)PEN_TYPE::END>		m_arrPen;		// ���� ����� ���� �迭
	CS_KEYBOARD_INPUT_PACKET m_inputkey[(int)KEY::LAST];	// Ű �Է� ����

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void CreateBrushPen();
	void Comunication();
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
	bool GetStart()						{ return m_bIsStart; }
	bool GetGameClear()					{ return m_bIsGameClear; }

	void SetSocket(SOCKET _sock)		{ m_sock = _sock; }
	void SetNickName(std::wstring _str)	{ m_strNickName = _str; }
	void SetID(int _iID)				{ m_iID = _iID; }
	void SetStart()						{ m_bIsStart = true; }
	void SetGameClear(bool _bIsClear)	{ m_bIsGameClear = _bIsClear; }
	void SetGameOver(bool _bIsOver)		{ m_bIsGameOver = _bIsOver; }

	CS_KEYBOARD_INPUT_PACKET GetKey(int i)	{ return m_inputkey[i]; }	// Ű ���� ��������
	void SetKey(int i, KEY_STATE key_state) { m_inputkey[i].inputs->key_state = key_state; }	// Ű ���� ����
};
