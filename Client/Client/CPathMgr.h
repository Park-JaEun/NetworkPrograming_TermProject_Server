#pragma once

class CPathMgr
{
	SINGLE(CPathMgr)
private:
	std::wstring	m_strResourcePath;	// 상대경로

public:
	void init();
	const std::wstring& GetResourcePath() { return m_strResourcePath; }
};

