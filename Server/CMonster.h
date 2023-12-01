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
    float		    m_fAttackCoolTime;
    MONSTER_STATE   m_eState;

    CLONE(CMonster)
public:
    void update() override;

public:
    float GetSpeed() { return m_fSpeed; }
    bool GetDir() { return m_bDir; }
    MONSTER_STATE GetState() { return m_eState; }

    void SetSpeed(const float& _f) { m_fSpeed = _f; }
    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }
    void SetMaxDistance(float _f) { m_fMaxDistance = _f; }
    void SetDir(bool _dir) { m_bDir = _dir; }
    void SetState(MONSTER_STATE _eState) { m_eState = _eState; }

private:
    void CreateBullet(int id, int bulletId);
    bool IsInSight(Vec2 _vPos, float _fDistance, const std::wstring& _strName);

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CMonster();
    ~CMonster();
};

