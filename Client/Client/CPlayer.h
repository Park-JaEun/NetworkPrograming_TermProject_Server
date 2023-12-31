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
    int             m_iCookieCount;     // 플레이어 쿠키 수
    int             m_iKillCount;       // 플레이어 킬 수
    float           m_fSpeed;           // 플레이어 속도
    float           m_fDieTime;         // 플레이어 사망 시간
    float           m_fResurrectTime;   // 플레이어 부활 시간
    bool            m_bDir;             // 플레이어 방향
    bool			m_bPrevDir;         // 플레이어 이전 방향
    bool			m_bIsGameOver;      // 플레이어 게임오버 여부
    PLAYER_STATE    m_eState;           // 플레이어 상태
    CHARACTER_TYPE  m_eType;            // 플레이어 타입
    CAnimator*      m_EffectAnimator;   // 이펙트 애니메이터
    Vec2			m_vPrevPos;         // 플레이어 이전 위치

    CLONE(CPlayer)
public:
    const CHARACTER_TYPE GetType()    { return m_eType; }
    const int GetLife()               { return m_iLife; }
    const int GetBunnyCount()         { return m_iBunnyCount; }
    const int GetCookieCount()        { return m_iCookieCount; }
    const int GetKillCount()          { return m_iKillCount; }
    const bool GetIsGameOver()        { return m_bIsGameOver; }
    const Vec2 GetPrevPos()           { return m_vPrevPos; }

    void SetType(CHARACTER_TYPE _eType) { m_eType = _eType; }
    void SetState(PLAYER_STATE state)   { m_eState = state; }
    void SetDir(bool dir)               { m_bDir = dir; }
    void SetLife(int life)              { m_iLife = life; }
    void SetHP(int hp)                  { m_iHP = hp; }
    void SetBunnyCount(int count)       { m_iBunnyCount = count; }
    void SetCookieCount(int count)      { m_iCookieCount = count; }
    void SetKillCount(int count)        { m_iKillCount = count; }
    void SetIsGameOver(bool isGameOver) { m_bIsGameOver = isGameOver; }

    void PlayShootingEffect();

    void update() override;
    void render(HDC _dc) override;

private:
    void CreateBullet(int id);

    void CreateMinji();
    void CreateHannie();
    void CreateDanielle();
    void CreateHaerin();
    void CreateHyein();

    void PlayAnimation();

    void PredictPlayerPos();  // 플레이어 사이드 스크롤링 예측
    void InterpolatePos();    // 플레이어 사이드 스크롤링 보간

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

