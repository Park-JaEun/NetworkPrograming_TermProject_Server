#pragma once

class CTexture;
class CSound;
class CResourceMgr
{
	SINGLE(CResourceMgr)
private:
	std::map<std::wstring, CTexture*>	m_mapTex;
	std::map<std::wstring, CSound*>		m_mapSound;

public:
	CTexture* LoadTexture(const std::wstring& _strKey, const std::wstring& _strRelativePath);
	CTexture* FindTexture(const std::wstring& _strKey);

	CSound* LoadSound(const std::wstring& _strKey, const std::wstring& _strRelativePath);
	CSound* FindSound(const std::wstring& _strkey);
};
