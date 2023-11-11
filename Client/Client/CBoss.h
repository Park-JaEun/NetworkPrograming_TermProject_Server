#pragma once
#include "CObject.h"

class CAnimator;

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
    CAnimator*      m_EffectAnimator;

    CLONE(CBoss)
public:
    void update() override;
    void render(HDC _dc) override;

public:
    bool GetHaveToAppear() { return m_bHaveToAppear; }
    bool GetIsAppear() { return m_bIsAppear; }

    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }
    void SetHaveToAppear(bool _bHaveToAppear) { m_bHaveToAppear = _bHaveToAppear; }
    void SetIsAppear(bool _bIsAppear) { m_bIsAppear = _bIsAppear; }

private:
    void CreateFanBullet();
    void CreateMissile();

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    void CreateAnimator() override;

public:
    CBoss();
    ~CBoss();
};

