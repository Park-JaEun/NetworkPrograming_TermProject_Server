#pragma once

class CResource
{
private:
	std::wstring	m_strKey;			// ���ҽ� Ű��
	std::wstring	m_strRelativePath;	// ���ҽ� ���(�����)

public:
	const std::wstring& GetKey()							{ return m_strKey; }
	const std::wstring& GetRelativePath()					{ return m_strRelativePath; }

	void SetKey(std::wstring _strKey)						{ m_strKey = _strKey; }
	void SetRelativePath(std::wstring _strRelativePath)		{ m_strRelativePath = _strRelativePath; }

public:
	CResource();
	virtual ~CResource();
};