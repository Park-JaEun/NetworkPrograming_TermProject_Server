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
    // 아무곳에서나 동적할당 할 수 없도록 생성자 private 선언
    CTexture();
    ~CTexture();

    friend class CResourceMgr;  // 리소스 매니저에서는 동적할당가능.
};

