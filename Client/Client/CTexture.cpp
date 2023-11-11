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
	// ���� ��ηκ��� �ε��� �����͸� ��Ʈ������ ����
	m_hBitmap = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, false, false, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// m_bitmap�� nullptr�̸� ����
	assert(m_hBitmap);

	// ��Ʈ�ʰ� ������ DC�� �޾ƿ�
	m_hDC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	// ��Ʈ�ʰ� DC ����
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
	DeleteObject(hOldBit);

	// ��Ʈ�� ���� �޾ƿ���
	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitInfo);
}