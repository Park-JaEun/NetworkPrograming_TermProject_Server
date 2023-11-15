#include "pch.h"
#include "CResourceMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"

CResourceMgr::CResourceMgr()
{

}

CResourceMgr::~CResourceMgr()
{
	for (auto _mapIter = m_mapTex.begin(); _mapIter != m_mapTex.end(); ++_mapIter)
		if (_mapIter->second != nullptr)
			delete _mapIter->second;
	m_mapTex.clear();
}

CTexture* CResourceMgr::LoadTexture(const std::wstring& _strKey, const std::wstring& _strRelativePath)
{
	CTexture* pTexture = FindTexture(_strKey);

	// 이미 있는 텍스처면 그 텍스처를 반환
	if (pTexture != nullptr)
		return pTexture;

	// 아니면 텍스처 load
	std::wstring strFilePath = CPathMgr::GetInst()->GetResourcePath();
	strFilePath += _strRelativePath;

	// texture load
	pTexture = new CTexture;
	pTexture->Load(strFilePath);

	// texture 정보 등록
	pTexture->SetKey(_strKey);
	pTexture->SetRelativePath(_strRelativePath);

	// resource manager에 추가
	m_mapTex.insert(std::make_pair(_strKey, pTexture));

	return pTexture;
}

CTexture* CResourceMgr::FindTexture(const std::wstring& _strKey)
{
	std::map<std::wstring, CTexture*>::iterator _mapIter = m_mapTex.find(_strKey);

	if (_mapIter == m_mapTex.end()) {
		return nullptr;
	}

	return _mapIter->second;
}
