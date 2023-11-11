#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr() : m_strResourcePath{}
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::init()
{
	// ���α׷��� ��θ� �޾ƿ´�. (������Ʈ �Ӽ����� ��� ���丮�� �۾� ���丮�� ��������)
	WCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);

	// ��θ� std::wstring���� ��ȯ
	std::wstring currentPath(buffer);

	// 1�� ���������� �̵�
	size_t lastSlash = currentPath.find_last_of(L"\\");
	if (lastSlash != std::wstring::npos)
		currentPath = currentPath.substr(0, lastSlash);

	// "\\bin\\resource\\" �߰�
	currentPath += L"\\bin\\resource\\";

	// ������ ��θ� m_szResourcePath�� �Ҵ�
	m_strResourcePath = currentPath;
}
