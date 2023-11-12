#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // 플레이어 체력
    float           m_fSpeed;           // 플레이어 속도
    float           m_fDieTime;         // 플레이어 최대 이동 거리
    bool            m_bDir;             // 플레이어 방향
    PLAYER_STATE    m_eState;           // 플레이어 상태
    CAnimator*      m_EffectAnimator;   // 이펙트 애니메이터

    CLONE(CPlayer)
public:
    void update() override;
    void render(HDC _dc) override;

private:
    void CreateBullet();

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;
    
public:
    void CreateAnimator() override;

public:
    CPlayer();
    ~CPlayer();
};

