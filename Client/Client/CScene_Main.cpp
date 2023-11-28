#include "pch.h"
#include "CSceneMgr.h"
#include "CScene_Main.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CBoss.h"
#include "CBackground.h"
#include "CFloor.h"
#include "CCore.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CCamera.h"
#include "CUI.h"
#include "CItem.h"

CScene_Main::CScene_Main() : m_bIsBoss(false)
{
}

CScene_Main::~CScene_Main()
{
}

void CScene_Main::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	///////////////////////
	// Background Object //
	///////////////////////
	CObject* pBGObj = new CBackground;
	pBGObj->SetName(L"Main Background");
	pBGObj->SetPos(Vec2(0.f, 0.f));
	pBGObj->SetScale(Vec2(vResolution.x, vResolution.y));

	// 애니메이터 생성
	pBGObj->CreateAnimator();

	CreateObject(pBGObj, GROUP_TYPE::BACKGROUND);
	///////////////////////


	/////////////////
	// Boss Object //
	/////////////////
	CObject* pBossObj = new CBoss;

	pBossObj->SetName(L"Boss");
	pBossObj->SetPos(Vec2(5250.f, 360.f));

	// 충돌체, 애니메이터 생성
	pBossObj->CreateCollider();
	pBossObj->CreateAnimator();

	pBossObj->GetCollider()->SetScale(Vec2(33.f * 2.f, 64.f * 4.6f));

	CreateObject(pBossObj, GROUP_TYPE::BOSS);
	/////////////////

	///////////////////
	// Player Object //
	///////////////////
	CObject* pPlayerObj = new CPlayer;

	pPlayerObj->SetName(L"Player");
	pPlayerObj->SetPos(Vec2(0.f, 0.f));

	switch (m_eSelectedCharacter)
	{
	case CHARACTER_TYPE::MINJI:
		((CPlayer*)pPlayerObj)->SetType(CHARACTER_TYPE::MINJI);
		break;
	case CHARACTER_TYPE::HANNIE:
		((CPlayer*)pPlayerObj)->SetType(CHARACTER_TYPE::HANNIE);
		break;
	case CHARACTER_TYPE::DANIELLE:
		((CPlayer*)pPlayerObj)->SetType(CHARACTER_TYPE::DANIELLE);
		break;
	case CHARACTER_TYPE::HAERIN:
		((CPlayer*)pPlayerObj)->SetType(CHARACTER_TYPE::HAERIN);
		break;
	case CHARACTER_TYPE::HYEIN:
		((CPlayer*)pPlayerObj)->SetType(CHARACTER_TYPE::HYEIN);
		break;
	default:
		break;
	}

	// 충돌체, 애니메이터 생성
	pPlayerObj->CreateCollider();
	pPlayerObj->CreateAnimator();

	pPlayerObj->GetCollider()->SetScale(Vec2(31.f, 30.f));

	CreateObject(pPlayerObj, GROUP_TYPE::PLAYER);
	///////////////////
	
	///////////////
	// UI Object //
	///////////////

	// Profile UI
	CUI* pProfileUI = new CUI;
	pProfileUI->SetName(L"Profile");
	pProfileUI->CreateAnimator();
	AddObject(pProfileUI, GROUP_TYPE::UI);

	// HP UI
	CUI* pHPUI = new CUI;
	pHPUI->SetName(L"HP");
	pHPUI->CreateAnimator();
	AddObject(pHPUI, GROUP_TYPE::UI);

	// Life Text UI
	CUI* pLifeTextUI = new CUI;
	pLifeTextUI->SetName(L"Life Text");
	pLifeTextUI->CreateAnimator();
	AddObject(pLifeTextUI, GROUP_TYPE::UI);

	// Score Text UI
	CUI* pScoreTextUI = new CUI;
	pScoreTextUI->SetName(L"Score Text");
	pScoreTextUI->CreateAnimator();
	AddObject(pScoreTextUI, GROUP_TYPE::UI);

	// Bunny Score UI
	CUI* pBunnyScoreUI = new CUI;
	pBunnyScoreUI->SetName(L"Bunny Score");
	AddObject(pBunnyScoreUI, GROUP_TYPE::UI);

	// Cookie Score UI
	CUI* pCookieScoreUI = new CUI;
	pCookieScoreUI->SetName(L"Cookie Score");
	AddObject(pCookieScoreUI, GROUP_TYPE::UI);
	
	// Kill Score UI
	CUI* pKillScoreUI = new CUI;
	pKillScoreUI->SetName(L"Kill Score");
	AddObject(pKillScoreUI, GROUP_TYPE::UI);

	//////////////
	
	//////////////////
	// Floor Object //
	//////////////////
	CreateFloor(Vec2(800.f, -40.f));
	CreateFloor(Vec2(910.f, 100.f));

	CreateFloor(Vec2(1440.f, -100.f));
	CreateFloor(Vec2(1440.f, 100.f));
	CreateFloor(Vec2(1440.f, 0.f));

	CreateFloor(Vec2(2010.f, -100.f));
	CreateFloor(Vec2(2130.f, 100.f));
	CreateFloor(Vec2(2250.f, 0.f));

	CreateFloor(Vec2(2770.f, 100.f));
	CreateFloor(Vec2(2890.f, 0.f));
	CreateFloor(Vec2(3010.f, -100.f));

	CreateFloor(Vec2(3460.f, -100.f));
	CreateFloor(Vec2(3660.f, 0.f));
	CreateFloor(Vec2(3660.f, 120.f));
	CreateFloor(Vec2(3840.f, -100.f));

	CreateFloor(Vec2(4340.f, 0.f));
	CreateFloor(Vec2(4200.f, -100.f));
	CreateFloor(Vec2(4200.f, 100.f));
	CreateFloor(Vec2(4480.f, -100.f));
	CreateFloor(Vec2(4480.f, 100.f));
	//////////////////


	////////////////////
	// Monster Object //
	////////////////////
	CreateMonster(Vec2(800.f, -90.f));
	CreateMonster(Vec2(910.f, 50.f));

	CreateMonster(Vec2(1440.f, -150.f));
	CreateMonster(Vec2(1440.f, 50.f));
	CreateMonster(Vec2(1440.f, -50.f));

	CreateMonster(Vec2(2010.f, -150.f));
	CreateMonster(Vec2(2130.f, 50.f));
	CreateMonster(Vec2(2250.f, -50.f));

	CreateMonster(Vec2(2770.f, 50.f));
	CreateMonster(Vec2(2890.f, -50.f));
	CreateMonster(Vec2(3010.f, -150.f));

	CreateMonster(Vec2(3460.f, -150.f));
	CreateMonster(Vec2(3660.f, -50.f));
	CreateMonster(Vec2(3660.f, 70.f));
	CreateMonster(Vec2(3840.f, -150.f));

	CreateMonster(Vec2(4340.f, -50.f));
	CreateMonster(Vec2(4200.f, -150.f));
	CreateMonster(Vec2(4200.f, 50.f));
	CreateMonster(Vec2(4480.f, -150.f));
	CreateMonster(Vec2(4480.f, 50.f));
	////////////////////

	/////////////////
	// Item Object //
	/////////////////
	CreateBunny(Vec2(710.f, -83.f - 25.f));
	CreateBunny(Vec2(1350.f, -140.f - 25.f));
	CreateBunny(Vec2(1355.f, 60.f - 25.f));
	CreateBunny(Vec2(2165.f, -40.f - 25.f));
	CreateBunny(Vec2(2855.f, 60.f - 25.f));
	CreateBunny(Vec2(3375.f, -140.f - 25.f));
	CreateBunny(Vec2(3925.f, -140.f - 25.f));
	CreateBunny(Vec2(3660.f, 80.f - 25.f));
	CreateBunny(Vec2(4195.f, 60.f - 25.f));
	CreateBunny(Vec2(4480.f, 60.f - 25.f));
	CreateBunny(Vec2(4340.f, -40.f - 25.f));
	CreateBunny(Vec2(4480.f, -140.f - 25.f));
	CreateBunny(Vec2(4195.f, -140.f - 25.f));

	CreateCookie(Vec2(880.f, -80.f - 25.f));
	CreateCookie(Vec2(760.f, 100.f - 25.f));
	CreateCookie(Vec2(1525.f, -40.f - 25.f));
	CreateCookie(Vec2(2095.f, -150.f - 25.f));
	CreateCookie(Vec2(2215.f, 60.f - 25.f));
	CreateCookie(Vec2(2975.f, -40.f - 25.f));
	CreateCookie(Vec2(3093.f, -140.f - 25.f));
	CreateCookie(Vec2(3660.f, -40.f - 25.f));
	CreateCookie(Vec2(5065.f, 0.f - 25.f));
	/////////////////


	// 충돌 그룹 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::BOSS);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_BOSS, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_MONSTER, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MISSILE_BOSS, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::ITEM, GROUP_TYPE::PLAYER);

	// Camera Look 지정
	//CCamera::GetInst()->SetLookAt(Vec2(5060.f, 0.f));
	CCamera::GetInst()->SetTarget(pPlayerObj);	// Camera가 Player를 따라다니도록 지정

	// 통신에 필요한 변수들
	SOCKET sock = CCore::GetInst()->GetSocket();
	int retval{};
	int size{};

	// 플레이어 2명의 정보 수신
	char buf[BUFSIZE]{};

	////////////////////
	// Player2 Object //
	////////////////////
	retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = recv(sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	SC_PLAYER_PACKET* playerPacket = reinterpret_cast<SC_PLAYER_PACKET*>(buf);

	CObject* pPlayer2Obj = new CPlayer;

	pPlayer2Obj->SetName(L"Player2");
	pPlayer2Obj->SetPos(playerPacket->playerPos);

	switch (playerPacket->character)
	{ 
	case CHARACTER_TYPE::MINJI:
		((CPlayer*)pPlayer2Obj)->SetType(CHARACTER_TYPE::MINJI);
		break;
	case CHARACTER_TYPE::HANNIE:
		((CPlayer*)pPlayer2Obj)->SetType(CHARACTER_TYPE::HANNIE);
		break;
	case CHARACTER_TYPE::DANIELLE:
		((CPlayer*)pPlayer2Obj)->SetType(CHARACTER_TYPE::DANIELLE);
		break;
	case CHARACTER_TYPE::HAERIN:
		((CPlayer*)pPlayer2Obj)->SetType(CHARACTER_TYPE::HAERIN);
		break;
	case CHARACTER_TYPE::HYEIN:
		((CPlayer*)pPlayer2Obj)->SetType(CHARACTER_TYPE::HYEIN);
		break;
	default:
		break;
	}

	// 충돌체, 애니메이터 생성
	pPlayer2Obj->CreateCollider();
	pPlayer2Obj->CreateAnimator();

	pPlayer2Obj->GetCollider()->SetScale(Vec2(31.f, 30.f));

	CreateObject(pPlayer2Obj, GROUP_TYPE::PLAYER);
	///////////////////

	////////////////////
	// Player3 Object //
	////////////////////

	retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = recv(sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}

	playerPacket = reinterpret_cast<SC_PLAYER_PACKET*>(buf);

	CObject* pPlayer3Obj = new CPlayer;

	pPlayer3Obj->SetName(L"Player3");
	pPlayer3Obj->SetPos(Vec2(0.f, 0.f));

	switch (playerPacket->character)
	{
	case CHARACTER_TYPE::MINJI:
		((CPlayer*)pPlayer3Obj)->SetType(CHARACTER_TYPE::MINJI);
		break;
	case CHARACTER_TYPE::HANNIE:
		((CPlayer*)pPlayer3Obj)->SetType(CHARACTER_TYPE::HANNIE);
		break;
	case CHARACTER_TYPE::DANIELLE:
		((CPlayer*)pPlayer3Obj)->SetType(CHARACTER_TYPE::DANIELLE);
		break;
	case CHARACTER_TYPE::HAERIN:
		((CPlayer*)pPlayer3Obj)->SetType(CHARACTER_TYPE::HAERIN);
		break;
	case CHARACTER_TYPE::HYEIN:
		((CPlayer*)pPlayer3Obj)->SetType(CHARACTER_TYPE::HYEIN);
		break;
	default:
		break;
	}

	// 충돌체, 애니메이터 생성
	pPlayer3Obj->CreateCollider();
	pPlayer3Obj->CreateAnimator();

	pPlayer3Obj->GetCollider()->SetScale(Vec2(31.f, 30.f));

	CreateObject(pPlayer3Obj, GROUP_TYPE::PLAYER);
	///////////////////


	// 서버에게 초기화 완료 패킷 송신
	CS_INIT_FINISH_PACKET initFinishPacket;
	size = sizeof(CS_INIT_FINISH_PACKET);
	

	initFinishPacket.type = static_cast<char>(CS_PACKET_TYPE::CS_INIT_FINISH);
	initFinishPacket.id = CCore::GetInst()->GetID();

	retval = send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(sock, reinterpret_cast<char*>(&initFinishPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		closesocket(sock);
		WSACleanup();
		return;
	}
	std::cout << "초기화 완료 패킷 송신" << std::endl;

	std::cout << "게임 시작 패킷 수신 대기중" << std::endl;
	// 게임 시작 신호 수신
	memset(buf, 0, BUFSIZE);
	retval = recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = recv(sock, buf, size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		closesocket(sock);
		WSACleanup();
		return;
	}
	SC_GAME_START_PACKET* initPacket = reinterpret_cast<SC_GAME_START_PACKET*>(buf);

	if (initPacket->type == static_cast<char>(SC_PACKET_TYPE::SC_GAME_START)) {
		CCore::GetInst()->SetStart();
		std::cout << "게임 시작 패킷 수신" << std::endl;
	}
	else {
		std::cout << "게임 시작 패킷 수신 오류: Packet Type이" << initPacket->type << "입니다." << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

}

void CScene_Main::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInst()->Reset();
}

void CScene_Main::update()
{
	CScene::update();

	// 플레이어 x위치가 5060보다 크거나 같으면, 카메라를 5060으로 고정
	CObject* pPlayer = FindObject(L"Player");
	if (pPlayer == nullptr)
		return;

	CBoss* pBoss = (CBoss*)FindObject(L"Boss");
	if (pBoss == nullptr)
		return;
	
	Vec2 vPlayerPos = pPlayer->GetPos();

	if (vPlayerPos.x >= 5060.f && CCamera::GetInst()->GetTarget() == pPlayer) {
		CCamera::GetInst()->SetLookAt(Vec2(5060.f, 0.f));
		CCamera::GetInst()->SetTarget(nullptr);
		SetIsBoss(true);
		pBoss->SetHaveToAppear(true);
	}

	// TODO: Player Life가 0이 되면, GameOver


	// + @

}
