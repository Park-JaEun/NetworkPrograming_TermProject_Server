#include "pch.h"
#include "CTexture.h"

#include "CCore.h"

CTexture::CTexture() : m_hDC(nullptr), m_hBitmap(nullptr), m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_hDC);
	DeleteObject(m_hBitmap);
}

void CTexture::Load(const std::wstring& _strFilePath)
{
	// 파일 경로로부터 로딩한 데이터를 비트맵으로 생성
	m_hBitmap = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, false, false, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// m_bitmap이 nullptr이면 오류
	assert(m_hBitmap);

	// 비트맵과 연결할 DC를 받아옴
	m_hDC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	// 비트맵과 DC 연결
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
	DeleteObject(hOldBit);

	// 비트맵 정보 받아오기
	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitInfo);
}