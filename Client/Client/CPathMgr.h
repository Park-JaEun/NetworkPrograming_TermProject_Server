#pragma once

class CPathMgr
{
	SINGLE(CPathMgr)
private:
	std::wstring	m_strResourcePath;	// �����

public:
	void init();
	const std::wstring& GetResourcePath() { return m_strResourcePath; }
};

