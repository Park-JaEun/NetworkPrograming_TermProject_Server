#pragma once
#include "CObject.h"

class CItem :
    public CObject
{
private:
    int             m_iID;
    Vec2            m_vFirstPos;
    float           m_fSpeed;
    float           m_fMaxDistance;
    bool            m_bDir;
    GROUP_TYPE      m_eGroupType;

    CLONE(CItem)
public:
    void update() override;

public:
    const int GetID() { return m_iID; }
    const GROUP_TYPE GetGroupType() { return m_eGroupType; }

    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }
    void SetID(int _iID) { m_iID = _iID; }
    void SetGroupType(GROUP_TYPE _eGroupType) { m_eGroupType = _eGroupType; }

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CItem();
    ~CItem();
};