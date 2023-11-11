#pragma once
#include "CResource.h"

class CTexture :
    public CResource
{
private:
    HDC         m_hDC;
    HBITMAP     m_hBitmap;
    BITMAP      m_bitInfo;

public:
    void Load(const std::wstring& _strFilePath);

    const UINT GetWidth()   { return m_bitInfo.bmWidth; }
    const UINT GetHeight()  { return m_bitInfo.bmHeight; }
    HDC GetDC()             { return m_hDC; }
    
private:    
    // �ƹ��������� �����Ҵ� �� �� ������ ������ private ����
    CTexture();
    ~CTexture();

    friend class CResourceMgr;  // ���ҽ� �Ŵ��������� �����Ҵ簡��.
};

