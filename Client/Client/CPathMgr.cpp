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
	// 프로그램의 경로를 받아온다. (프로젝트 속성에서 출력 디렉토리와 작업 디렉토리를 변경했음)
	WCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);

	// 경로를 std::wstring으로 변환
	std::wstring currentPath(buffer);

	// 1번 상위폴더로 이동
	size_t lastSlash = currentPath.find_last_of(L"\\");
	if (lastSlash != std::wstring::npos)
		currentPath = currentPath.substr(0, lastSlash);

	// "\\bin\\resource\\" 추가
	currentPath += L"\\bin\\resource\\";

	// 수정된 경로를 m_szResourcePath에 할당
	m_strResourcePath = currentPath;
}
