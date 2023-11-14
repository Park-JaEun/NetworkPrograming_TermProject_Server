#pragma once

class CResource
{
private:
	std::wstring	m_strKey;			// 리소스 키값
	std::wstring	m_strRelativePath;	// 리소스 경로(상대적)

public:
	const std::wstring& GetKey()							{ return m_strKey; }
	const std::wstring& GetRelativePath()					{ return m_strRelativePath; }

	void SetKey(std::wstring _strKey)						{ m_strKey = _strKey; }
	void SetRelativePath(std::wstring _strRelativePath)		{ m_strRelativePath = _strRelativePath; }

public:
	CResource();
	virtual ~CResource();
};