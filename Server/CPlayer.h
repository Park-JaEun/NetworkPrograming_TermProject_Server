#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // 체력
    int             m_iLife;            // 목숨
    int             m_iBunnyCount;      // 토끼 획득 개수
    int             m_iCookieCount;     // 쿠키 획득 개수
    int             m_iKillCount;       // 킬 개수 
    float           m_fSpeed;           // 스피드 
    float           m_fDieTime;         // 죽은 시간
    float           m_fResurrectTime;   // 부활 시간
    bool            m_bDir;             // 방향
    bool			m_bIsGameOver;      // 게임오버 유무
    PLAYER_STATE    m_eState;           // 상태
    CHARACTER_TYPE  m_eType;            // 캐릭터 

    CLONE(CPlayer)
public:
    float GetSpeed()                      {      return m_fSpeed;    }
    bool GetDir()                       {        return m_bDir; }
    PLAYER_STATE GetState()             { return m_eState; }

    void SetState(PLAYER_STATE state)    { m_eState = state; }
    void SetDir(bool dir)               { m_bDir = dir; }

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

    void CreateBullet(int id, int bulletId);

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CPlayer();
    ~CPlayer();
};