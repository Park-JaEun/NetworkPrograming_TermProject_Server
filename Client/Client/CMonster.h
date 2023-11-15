#pragma once
#include "CObject.h"

class CMonster :
    public CObject
{
private:
    Vec2            m_vFirstPos;
    float           m_fSpeed;
    float           m_fMaxDistance;
    bool            m_bDir;
    int             m_iHP;
    MONSTER_STATE   m_eState;

    CLONE(CMonster)
public:
    void update() override;
    void render(HDC _dc) override;

public:
    float GetSpeed()                { return m_fSpeed; }
    void SetSpeed(const float& _f)  { m_fSpeed = _f; }

    void SetFirstPos(Vec2 _vPos)   { m_vFirstPos = _vPos; }
    void SetMaxDistance(float _f)   { m_fMaxDistance = _f; }

private:
    void CreateBullet();
    bool IsInSight(Vec2 _vPos, float _fDistance, const std::wstring& _strName);

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    void CreateAnimator() override;

public:
    CMonster();
    ~CMonster();
};

