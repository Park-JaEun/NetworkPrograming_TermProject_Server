#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"

#include "CKeyMgr.h"
#include "CTimer.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Main.h"

#include "CResourceMgr.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CAnimation.h"

CPlayer::CPlayer() : m_bDir(DIR_RIGHT), m_eState(PLAYER_STATE::IDLE), 
					 m_EffectAnimator(nullptr), m_fSpeed(300.f), m_iHP(5),
					 m_fDieTime(0.f)
{
}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	Vec2 vPos = GetPos();
	Vec2 vDummyPos{};
	CScene_Main* pMainScene = (CScene_Main*)CSceneMgr::GetInst()->GetCurScene();
	bool bIsBoss = pMainScene->GetIsBoss();

	if (m_iHP <= 0) {
		if (m_bDir == DIR_RIGHT)
			GetAnimator()->Play(L"Haerin_Die_Right", true);
		else if (m_bDir == DIR_LEFT)
			GetAnimator()->Play(L"Haerin_Die_Left", true);

		// 1.5초뒤 아래로 추락
		if(m_fDieTime >= 1.5f)
			vPos.y += DT * 50.f * 3;
		else
			m_fDieTime += DT;
	}
	else {
		// 상
		if (KEY_HOLD(KEY::UP)) {
			vDummyPos = Vec2(vPos.x, vPos.y - m_fSpeed * DT);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.y -= m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.y -= m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
		}
		if (KEY_AWAY(KEY::UP)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 하
		if (KEY_HOLD(KEY::DOWN)) {
			vDummyPos = Vec2(vPos.x, vPos.y + m_fSpeed * DT);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.y += m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.y += m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
		}
		if (KEY_AWAY(KEY::DOWN)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 좌
		if (KEY_HOLD(KEY::LEFT)) {
			vDummyPos = Vec2(vPos.x - m_fSpeed * DT, vPos.y);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.x -= m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.x -= m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
			if (m_bDir != DIR_LEFT)
				m_bDir = DIR_LEFT;
		}
		if (KEY_AWAY(KEY::LEFT)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		// 우
		if (KEY_HOLD(KEY::RIGHT)) {
			vDummyPos = Vec2(vPos.x + m_fSpeed * DT, vPos.y);

			if (IsInWorld(vDummyPos) && !bIsBoss)
				vPos.x += m_fSpeed * DT;
			else if (IsInBossRoom(vDummyPos) && bIsBoss)
				vPos.x += m_fSpeed * DT;

			m_eState = PLAYER_STATE::RUN;
			if (m_bDir != DIR_RIGHT)
				m_bDir = DIR_RIGHT;
		}
		if (KEY_AWAY(KEY::RIGHT)) {
			m_eState = PLAYER_STATE::IDLE;
		}

		if (KEY_TAP(KEY::SPACE)) {
			// 총알 발사
			CreateBullet();

			// 슈팅 이펙트 재생
			m_EffectAnimator->Play(L"Shooting", false);
			m_EffectAnimator->FindAnimation(L"Shooting")->SetFrame(0);
		}

		// 플레이어 상태와 방향에 따라서 플레이되는 애니메이션 변경
		if (m_bDir == DIR_RIGHT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Haerin_Idle_Right", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Haerin_Run_Right", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			}
		}
		else if (m_bDir == DIR_LEFT) {
			switch (m_eState)
			{
			case PLAYER_STATE::IDLE:
				GetAnimator()->Play(L"Haerin_Idle_Left", true);
				break;
			case PLAYER_STATE::RUN:
				GetAnimator()->Play(L"Haerin_Run_Left", true);
				break;
			case PLAYER_STATE::HIT:
				// TODO: HIT Animation 추가
				break;
			}
		}

		if (m_EffectAnimator != nullptr)
			m_EffectAnimator->update();
	}

	SetPos(vPos);				// 위치 업데이트
	GetAnimator()->update();	// 애니메이터 업데이트
}

void CPlayer::render(HDC _dc)
{
	if (m_EffectAnimator != nullptr)
		m_EffectAnimator->render(_dc);

	// 컴포넌트 그리기(충돌체, 애니메이션)
	componentRender(_dc);
}

void CPlayer::CreateBullet()
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Player Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetScale(Vec2(14.f, 13.f));
	
	if (m_bDir == DIR_RIGHT)
		pBullet->SetDir(DIR_RIGHT);
	else
		pBullet->SetDir(DIR_LEFT);

	pBullet->CreateCollider();
	pBullet->CreateAnimator(GROUP_TYPE::BULLET_PLAYER);

	pBullet->SetSpeed(700.f);
	pBullet->GetCollider()->SetScale(Vec2(25.f, 25.f));

	CreateObject(pBullet, GROUP_TYPE::BULLET_PLAYER);
}

void CPlayer::CreateAnimator()
{
	// Texture 로딩하기
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Haerin", L"texture\\characters\\haerin\\hearin.bmp");
	//CTexture* pTexture2 = CResourceMgr::GetInst()->LoadTexture(L"test", L"texture\\background\\"STA);

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;
	m_EffectAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);
	m_EffectAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Haerin_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(27.f, 31.f),			// Frame Size
		Vec2(0.f, 0.f),				// Step
		0.f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		1);							// Frame Count
	
	pAnimator->CreateAnimation(
		L"Haerin_Idle_Left",
		pTexture,
		Vec2(0.f, 91.f),
		Vec2(27.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Haerin_Run_Right",	
		pTexture,
		Vec2(0.f, 61.f),			
		Vec2(31.f, 30.f),			
		Vec2(0.f, 0.f),				
		0.f,						
		Vec2(0.f, 0.f),				
		1);	

	pAnimator->CreateAnimation(
		L"Haerin_Run_Left",			
		pTexture,
		Vec2(0.f, 152.f),
		Vec2(31.f, 30.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Haerin_Die_Right",
		pTexture,
		Vec2(187.f, 0.f),
		Vec2(27.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);

	pAnimator->CreateAnimation(
		L"Haerin_Die_Left",
		pTexture,
		Vec2(187.f, 31.f),
		Vec2(27.f, 31.f),
		Vec2(0.f, 0.f),
		0.f,
		Vec2(0.f, 0.f),
		1);
	

	m_EffectAnimator->CreateAnimation(
		L"Shooting",			// Animation Name
		pTexture,				// Texture Pointer
		Vec2(0.f, 182.f),		// Texture Left Top
		Vec2(36.f, 35.f),		// Frame Size
		Vec2(36.f, 0.f),		// Step
		0.07f,					// Duration
		Vec2(0.f, 0.f),			// Offset
		Vec2(2.f, 2.f),			// Scale
		7);						// Frame Count

	pAnimator->Play(L"Haerin_Idle_Right", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CPlayer::OnCollision(CCollider* _pOther)
{
}

void CPlayer::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster Bullet") {
		--m_iHP;
	}

	if (pOtherObj->GetName() == L"Boss Bullet" || pOtherObj->GetName() == L"Boss Missile") {
		--m_iHP;
	}
}

void CPlayer::ExitCollision(CCollider* _pOther)
{
}
