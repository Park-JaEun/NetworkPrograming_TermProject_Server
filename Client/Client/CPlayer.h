#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // 플레이어 체력
    int             m_iLife;            // 플레이어 생명
    float           m_fSpeed;           // 플레이어 속도
    float           m_fDieTime;         // 플레이어 최대 이동 거리
    bool            m_bDir;             // 플레이어 방향
    PLAYER_STATE    m_eState;           // 플레이어 상태
    CHARACTER_TYPE  m_eType;            // 플레이어 타입
    CAnimator*      m_EffectAnimator;   // 이펙트 애니메이터

    CLONE(CPlayer)
public:
    CHARACTER_TYPE GetType() { return m_eType; }
    void SetType(CHARACTER_TYPE _eType) { m_eType = _eType; }

    void update() override;
    void render(HDC _dc) override;

private:
    void CreateBullet();

    void CreateMinji();
    void CreateHannie();
    void CreateDanielle();
    void CreateHaerin();
    void CreateHyein();

    void PlayAnimation();

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

