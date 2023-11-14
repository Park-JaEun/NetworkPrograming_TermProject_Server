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

CMonster::CMonster() : m_fSpeed(50.f), m_vFirstPos{}, m_fMaxDistance(50.f), m_bDir(DIR_RIGHT), m_iHP(3), m_eState(MONSTER_STATE::MOVE)
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	Vec2 vCurPos = GetPos();

	// HP가 0이하가 되면 죽는 애니메이션 재생후 삭제
	if (m_iHP <= 0) {
		if(m_eState != MONSTER_STATE::DIE)
			m_eState = MONSTER_STATE::DIE;

		if (GetAnimator()->FindAnimation(L"Bunny_Die_Left")->IsFinish() || GetAnimator()->FindAnimation(L"Bunny_Die_Right")->IsFinish())
			DeleteObject(this);
	}
	// HP가 0보다 높을 때
	else {
		// 본인 시야에 플레이어가 있으면 공격
		if (IsInSight(GetPos(), 300.f, L"Player")) {
			m_eState = MONSTER_STATE::ATTACK;

			if(m_bDir == DIR_RIGHT)
				GetAnimator()->Play(L"Bunny_Attack_Right", false);
			if (m_bDir == DIR_LEFT)
				GetAnimator()->Play(L"Bunny_Attack_Left", false);
		}
		// 플레이어가 없으면 다시 움직임
		else {
			m_eState = MONSTER_STATE::MOVE;
		}

		// 공격하고 있을 때
		if (m_eState == MONSTER_STATE::ATTACK) {
			// 공격 애니메이션 재생 후 총알 생성
			if (GetAnimator()->FindAnimation(L"Bunny_Attack_Right")->IsFinish()) {
				CreateBullet();
				GetAnimator()->FindAnimation(L"Bunny_Attack_Right")->SetFrame(0);
			}

			if (GetAnimator()->FindAnimation(L"Bunny_Attack_Left")->IsFinish()) {
				CreateBullet();
				GetAnimator()->FindAnimation(L"Bunny_Attack_Left")->SetFrame(0);
			}
		}

		// 움직이고 있을 때
		if (m_eState == MONSTER_STATE::MOVE) {

			if (m_bDir == DIR_RIGHT) {
				// 진행 방향으로 이동
				vCurPos.x += DT * m_fSpeed * 1;

				// 배회 거리 기준량을 넘었는지 확인
				float fDiff = abs(m_vFirstPos.x - vCurPos.x) - m_fMaxDistance;

				if (fDiff > 0.f) {
					// 방향 변경
					m_bDir = !m_bDir;
					// 넘어선 만큼 반대방향으로 이동
					vCurPos.x += fDiff * -1;
				}
			}

			if (m_bDir == DIR_LEFT) {
				// 진행 방향으로 이동
				vCurPos.x += DT * m_fSpeed * -1;

				// 배회 거리 기준량을 넘었는지 확인
				float fDiff = abs(m_vFirstPos.x - vCurPos.x) - m_fMaxDistance;

				if (fDiff > 0.f) {
					// 방향 변경
					m_bDir = !m_bDir;
					// 넘어선 만큼 반대방향으로 이동
					vCurPos.x += fDiff * 1;
				}
			}
		}
	}

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
			break;
		case MONSTER_STATE::DIE:
			GetAnimator()->Play(L"Bunny_Die_Left", false);
			break;
		default:
			break;
		}
	}

	SetPos(vCurPos);
	GetAnimator()->update();	// 애니메이터 업데이트
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


