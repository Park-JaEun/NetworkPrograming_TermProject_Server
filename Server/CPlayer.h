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

    CLONE(CPlayer)
public:
    float GetSpeed()                      {      return m_fSpeed;    }
    bool GetDir()                       {        return m_bDir; }
    PLAYER_STATE GetState()             { return m_eState; }

    void SetState(PLAYER_STATE state)    { m_eState = state; }
    void SetDir(bool dir)               { m_bDir = dir; }
    void update() override;

private:
    void CreateBullet();

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CPlayer();
    ~CPlayer();
};