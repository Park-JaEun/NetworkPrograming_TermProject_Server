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
    int 		    m_iID;
    int             m_iHP;
    float		    m_fAttackCoolTime;
    float           m_fDieTime;
    MONSTER_STATE   m_eState;
    GROUP_TYPE      m_eGroupType;

    CLONE(CMonster)
public:
    void update() override;

public:
    const float GetSpeed()          { return m_fSpeed; }
    const bool GetDir()             { return m_bDir; }
    const MONSTER_STATE GetState()  { return m_eState; }
    const int GetID()               { return m_iID; }
    const GROUP_TYPE GetGroupType() { return m_eGroupType; }

    void SetSpeed(const float& _f)          { m_fSpeed = _f; }
    void SetFirstPos(Vec2 _vPos)            { m_vFirstPos = _vPos; }
    void SetMaxDistance(float _f)           { m_fMaxDistance = _f; }
    void SetDir(bool _dir)                  { m_bDir = _dir; }
    void SetState(MONSTER_STATE _eState)    { m_eState = _eState; }
    void SetID(int _id)                     { m_iID = _id; }
    void SetGroupType(GROUP_TYPE _eGroup)   { m_eGroupType = _eGroup; }

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

