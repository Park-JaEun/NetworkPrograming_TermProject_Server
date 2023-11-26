#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // 플레이어 체력
    int             m_iLife;            // 플레이어 생명
    int             m_iBunnyCount;      // 플레이어 토끼 수
    int             m_iCookieCount;     // 플레이어 토끼 수
    int             m_iKillCount;       // 플레이어 킬 수
    float           m_fSpeed;           // 플레이어 속도
    float           m_fDieTime;         // 플레이어 사망 시간
    float           m_fResurrectTime;   // 플레이어 부활 시간
    bool            m_bDir;             // 플레이어 방향
    bool			m_bIsGameOver;      // 플레이어 게임오버 여부
    PLAYER_STATE    m_eState;           // 플레이어 상태
    CHARACTER_TYPE  m_eType;            // 플레이어 타입
    CAnimator*      m_EffectAnimator;   // 이펙트 애니메이터

    CLONE(CPlayer)
public:
    CHARACTER_TYPE GetType() { return m_eType; }
    int GetLife() { return m_iLife; }
    int GetBunnyCount() { return m_iBunnyCount; }
    int GetCookieCount() { return m_iCookieCount; }
    int GetKillCount() { return m_iKillCount; }

    void SetType(CHARACTER_TYPE _eType) { m_eType = _eType; }

    void PlusBunnyCount() { ++m_iBunnyCount; }
    void PlusCookieCount() { ++m_iCookieCount; }
    void PlusKillCount() { ++m_iKillCount; }

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

