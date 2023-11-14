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

	// �̹� �ִ� �ؽ�ó�� �� �ؽ�ó�� ��ȯ
	if (pTexture != nullptr)
		return pTexture;

	// �ƴϸ� �ؽ�ó load
	std::wstring strFilePath = CPathMgr::GetInst()->GetResourcePath();
	strFilePath += _strRelativePath;

	// texture load
	pTexture = new CTexture;
	pTexture->Load(strFilePath);

	// texture ���� ���
	pTexture->SetKey(_strKey);
	pTexture->SetRelativePath(_strRelativePath);

	// resource manager�� �߰�
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
