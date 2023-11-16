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

	// �ִϸ����� ����
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
		SELECT_CHARACTER_PACKET selectPacket;
		SOCKET sock = CCore::GetInst()->GetSocket();
		char buf[BUFSIZE]{};
		int retval{};
		int size = sizeof(SELECT_CHARACTER_PACKET);

		selectPacket.type = static_cast<char>(CS_PACKET_TYPE::SELECT_CHARACTER);
		selectPacket.character = m_eSelectedCharacter;
		selectPacket.id = CCore::GetInst()->GetID();

		retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = send(sock, reinterpret_cast<char*>(&selectPacket), size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		// ������ �Ϸ�Ǹ�, �ٸ� Ŭ���̾�Ʈ���� ������ �Ϸ��� �� ���� ���
		// ����, ��� Ŭ���̾�Ʈ�� ������ �Ϸ��ϸ�, �����κ��� �ʱ�ȭ ��ȣ�� ����
		retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		retval = recv(sock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			closesocket(sock);
			WSACleanup();
			return;
		}

		SC_INIT_PACKET* initPacket = reinterpret_cast<SC_INIT_PACKET*>(buf);
		if (initPacket->type == static_cast<char>(SC_PACKET_TYPE::SC_INIT))
			ChangeScene(SCENE_TYPE::MAIN);	// ENTER()���� �ʱ�ȭ ������ �Ѵ�.
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