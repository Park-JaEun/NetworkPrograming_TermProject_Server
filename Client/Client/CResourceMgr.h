#pragma once

class CTexture;

class CResourceMgr
{
	SINGLE(CResourceMgr)
private:
	std::map<std::wstring, CTexture*>	m_mapTex;

public:
	CTexture* LoadTexture(const std::wstring& _strKey, const std::wstring& _strRelativePath);
	CTexture* FindTexture(const std::wstring& _strKey);

};

