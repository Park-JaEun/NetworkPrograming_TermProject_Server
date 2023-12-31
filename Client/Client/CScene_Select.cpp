#include "pch.h"
#include "CScene_Select.h"
#include "CKeyMgr.h"

#include "CCore.h"
#include "CBackground.h"

#include "CUI.h"

CScene_Select::CScene_Select() : m_eSelectedCharacter(CHARACTER_TYPE::MINJI)
{
}

CScene_Select::~CScene_Select()
{
}

void CScene_Select::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	///////////////////////
	// Background Object //
	///////////////////////
	CObject* pBGObj = new CBackground;
	pBGObj->SetName(L"Select Background");
	pBGObj->SetPos(Vec2(0.f, 0.f));
	pBGObj->SetScale(Vec2(vResolution.x, vResolution.y));

	// 애니메이터 생성
	pBGObj->CreateAnimator();

	CreateObject(pBGObj, GROUP_TYPE::BACKGROUND);
	///////////////////////

	///////////////
	// UI Object //
	///////////////

	CUI* pSelectedCharacterUI = new CUI;
	pSelectedCharacterUI->SetName(L"Selected Character");
	pSelectedCharacterUI->SetPos(Vec2(0.f, 0.f));
	pSelectedCharacterUI->SetScale(Vec2(vResolution.x, vResolution.y));

	pSelectedCharacterUI->CreateAnimator();
	CreateObject(pSelectedCharacterUI, GROUP_TYPE::UI);
	////////////////
}

void CScene_Select::Exit()
{
	DeleteAll();
}

void CScene_Select::update()
{
	CScene::update();

	if (KEY_TAP(KEY::SPACE)) {
		
		SOCKET sock = CCore::GetInst()->GetSocket();
		char buf[BUFSIZE]{};
		int retval{};
		int size;
		
		sendSelectCharacter(sock);
		// 선택이 완료되면, 다른 클라이언트들이 선택을 완료할 때 까지 대기
		// 이후, 모든 클라이언트가 선택을 완료하면, 서버로부터 초기화 신호를 받음
		if (recvInitSignal(sock)) {
			ChangeScene(SCENE_TYPE::MAIN);	// ENTER()에서 초기화 동작을 한다.
		}
	}

	if (KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::DOWN)) {
		switch (m_eSelectedCharacter)
		{
		case CHARACTER_TYPE::MINJI:
			m_eSelectedCharacter = CHARACTER_TYPE::HAERIN;
			break;
		case CHARACTER_TYPE::HANNIE:
			m_eSelectedCharacter = CHARACTER_TYPE::HYEIN;
			break;
		case CHARACTER_TYPE::DANIELLE:
			m_eSelectedCharacter = CHARACTER_TYPE::HANNIE;
			break;
		case CHARACTER_TYPE::HAERIN:
			m_eSelectedCharacter = CHARACTER_TYPE::DANIELLE;
			break;
		case CHARACTER_TYPE::HYEIN:
			m_eSelectedCharacter = CHARACTER_TYPE::MINJI;
			break;
		default:
			break;
		}
	}
	if (KEY_TAP(KEY::RIGHT) || KEY_TAP(KEY::UP)) {
		switch (m_eSelectedCharacter)
		{
		case CHARACTER_TYPE::MINJI:
			m_eSelectedCharacter = CHARACTER_TYPE::HYEIN;
			break;
		case CHARACTER_TYPE::HANNIE:
			m_eSelectedCharacter = CHARACTER_TYPE::DANIELLE;
			break;
		case CHARACTER_TYPE::DANIELLE:
			m_eSelectedCharacter = CHARACTER_TYPE::HAERIN;
			break;
		case CHARACTER_TYPE::HAERIN:
			m_eSelectedCharacter = CHARACTER_TYPE::MINJI;
			break;
		case CHARACTER_TYPE::HYEIN:
			m_eSelectedCharacter = CHARACTER_TYPE::HANNIE;
			break;
		default:
			break;
		}
	}
}