#include "pch.h"
#include "CMonster.h"
#include "CBullet.h"

#include "CTimer.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CResourceMgr.h"
#include "CPathMgr.h"

#include "CTexture.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CMonster::CMonster() : m_fSpeed(50.f), m_vFirstPos{}, m_fMaxDistance(50.f), m_bDir(DIR_RIGHT), 
					   m_iHP(3), m_eState(MONSTER_STATE::MOVE), m_fDieTime(0.f), m_vPrevPos(Vec2(0.f, 0.f))
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	PredictMonsterPos();	// 예측
	InterpolatePos();	// 보간

	if (m_bDir == DIR_RIGHT) {
		switch (m_eState)
		{
		case MONSTER_STATE::IDLE:
			GetAnimator()->Play(L"Bunny_Idle_Right", true);
			break;
		case MONSTER_STATE::MOVE:
			GetAnimator()->Play(L"Bunny_Move_Right", true);
			break;
		case MONSTER_STATE::HIT:
			break;
		case MONSTER_STATE::ATTACK: 
		{
			GetAnimator()->Play(L"Bunny_Attack_Right", false);
			if(GetAnimator()->FindAnimation(L"Bunny_Attack_Right")->IsFinish())
				GetAnimator()->FindAnimation(L"Bunny_Attack_Right")->SetFrame(0);
		}
			break;
		case MONSTER_STATE::DIE:
			GetAnimator()->Play(L"Bunny_Die_Right", false);
			break;
		default:
			break;
		}
	}
	else if (m_bDir == DIR_LEFT) {
		switch (m_eState)
		{
		case MONSTER_STATE::IDLE:
			GetAnimator()->Play(L"Bunny_Idle_Left", true);
			break;
		case MONSTER_STATE::MOVE:
			GetAnimator()->Play(L"Bunny_Move_Left", true);
			break;
		case MONSTER_STATE::HIT:
			break;
		case MONSTER_STATE::ATTACK: 
		{
			GetAnimator()->Play(L"Bunny_Attack_Left", false);
			if(GetAnimator()->FindAnimation(L"Bunny_Attack_Left")->IsFinish())
				GetAnimator()->FindAnimation(L"Bunny_Attack_Left")->SetFrame(0);
		}
			break;
		case MONSTER_STATE::DIE:
			GetAnimator()->Play(L"Bunny_Die_Left", false);
			break;
		default:
			break;
		}
	}

	GetAnimator()->update();	// 애니메이터 업데이트

	// 죽었을 때, 이전 위치와 현재 위치가 변화가 없는 시간이 1.5초가 넘으면 존재하지 않는다고 판단
	if (m_eState == MONSTER_STATE::DIE) {
		if ((int)m_vPrevPos.x == (int)GetPos().x) {
			m_fDieTime += DT;
			if (m_fDieTime > 1.5f) {
				DeleteObject(this);
			}
		}
		else {
			m_fDieTime = 0.f;
			m_vPrevPos = GetPos();
		}
	}
	else {
		m_vPrevPos = GetPos();
	}
}

void CMonster::render(HDC _dc)
{
	componentRender(_dc);
}

void CMonster::CreateAnimator()
{
	CTexture* pTexture = CResourceMgr::GetInst()->LoadTexture(L"Bunny", L"texture\\characters\\bunny\\bunny.bmp");

	// Animator 만들기
	CAnimator* pAnimator = new CAnimator;

	// Animator에 Object 지정
	pAnimator->SetObj(this);

	// Animation 만들기
	pAnimator->CreateAnimation(
		L"Bunny_Idle_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 0.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Idle_Left",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 144.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Move_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 48.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.3f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		4);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Move_Left",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 192.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.3f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		4);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Attack_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 96.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Attack_Left",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 240.f),			// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.1f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		6);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Die_Right",		// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 306.f),				// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.05f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		20);							// Frame Count

	pAnimator->CreateAnimation(
		L"Bunny_Die_Left",			// Animation Name
		pTexture,					// Texture Pointer
		Vec2(0.f, 354.f),			// Texture Left Top
		Vec2(30.f, 48.f),			// Frame Size
		Vec2(30.f, 0.f),			// Step
		0.05f,						// Duration
		Vec2(0.f, 0.f),				// Offset
		20);						// Frame Count

	pAnimator->Play(L"Bunny_Move_Right", true);	// 현재 애니메이션 지정

	SetAnimator(pAnimator);
}

void CMonster::CreateBullet()
{
	Vec2 vBulletPos = GetPos();

	CBullet* pBullet = new CBullet;

	pBullet->SetName(L"Monster Bullet");
	pBullet->SetPos(vBulletPos);
	pBullet->SetFirstPos(vBulletPos);

	if (m_bDir == DIR_RIGHT)
		pBullet->SetDir(DIR_RIGHT);
	else
		pBullet->SetDir(DIR_LEFT);

	pBullet->CreateCollider();
	pBullet->CreateAnimator(GROUP_TYPE::BULLET_MONSTER);

	pBullet->SetSpeed(700.f);
	pBullet->GetCollider()->SetScale(Vec2(18.f, 18.f));

	CreateObject(pBullet, GROUP_TYPE::BULLET_MONSTER);
}

bool CMonster::IsInSight(Vec2 _vPos, float _fDistance, const std::wstring& _strName)
{
	// vPos를 기준으로 _fDistance 거리 안에 _strName 태그를 가진 오브젝트가 있는지 확인
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (pCurScene == nullptr)
		return false;

	std::vector<CObject*> vecObject = pCurScene->GetGroupObject(GROUP_TYPE::PLAYER);

	for (auto& pObj : vecObject) {
		if (pObj->GetName() == _strName) {
			Vec2 vOtherPos = pObj->GetPos();

			float fDistance = sqrtf(powf(_vPos.x - vOtherPos.x, 2) + powf(_vPos.y - vOtherPos.y, 2));

			if (fDistance < _fDistance) {
				// 대상이 오른에 있으면 DIR_RIGHT, 왼쪽에 있으면 DIR_LEFT
				if (_vPos.x - vOtherPos.x > 0)
					m_bDir = DIR_LEFT;
				else
					m_bDir = DIR_RIGHT;

				return true;
			}
		}
	}
	return false;
}

void CMonster::PredictMonsterPos()
{
	// 마지막 방향을 사용하여 몬스터의 위치 예측
	Vec2 vPos = GetPos();
	float fInterforate = 10.f;

	// 전 x좌표보다 현재 x좌표가 더 크다면
	if (vPos.x > m_vPrevPos.x)
	{
		vPos.x += (m_fSpeed / fInterforate) * DT;
	}
	else if (vPos.x < m_vPrevPos.x)
	{
		vPos.x -= (m_fSpeed / fInterforate) * DT;
	}

	SetPos(vPos);
}

void CMonster::InterpolatePos()
{
	// 보간 로직
	// 이전 포지션과 현재 포지션 상태 사이를 보간
	Vec2 vPos = GetPos();
	Vec2 vPrevPos = m_vPrevPos;
	Vec2 interpolatePos = vPos;

	// 보간 정도
	// 0 ~ 1 사이의 값
	float fInterpolate = 0.5f;

	interpolatePos.x = LerpX(vPrevPos.x, vPos.x, fInterpolate);
	interpolatePos.y = LerpY(vPrevPos.y, vPos.y, fInterpolate);

	SetPos(interpolatePos);
}

void CMonster::OnCollision(CCollider* _pOther)
{
}

void CMonster::EnterCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player Bullet") {
		// 플레이어 총알과 충돌시 HP 감소
		if (m_iHP > 0)
			m_iHP -= 1;
	}
}

void CMonster::ExitCollision(CCollider* _pOther)
{
}


