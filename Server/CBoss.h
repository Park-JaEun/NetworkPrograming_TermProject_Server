#pragma once
#include "CObject.h"

class CBoss :
    public CObject
{
private:
    int             m_iHP;
    float           m_fSpeed;
    float           m_fMaxDistance;
    float		    m_fAttackTime;
    float		    m_fDieTime;
    float           m_fDegree;
    Vec2			m_vFirstPos;
    bool            m_bHaveToAppear;
    bool			m_bIsAppear;
    BOSS_STATE		m_eState;

    CLONE(CBoss)
public:
    void update() override;

public:
    bool GetHaveToAppear()  { return m_bHaveToAppear; }
    bool GetIsAppear()      { return m_bIsAppear; }
    BOSS_STATE GetState()   { return m_eState; }

    void SetFirstPos(Vec2 _vPos)                { m_vFirstPos = _vPos; }
    void SetHaveToAppear(bool _bHaveToAppear)   { m_bHaveToAppear = _bHaveToAppear; }
    void SetIsAppear(bool _bIsAppear)           { m_bIsAppear = _bIsAppear; }
    void SetState(BOSS_STATE _eState)           { m_eState = _eState; }

private:
    void CreateFanBullet();

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CBoss();
    ~CBoss();
};

