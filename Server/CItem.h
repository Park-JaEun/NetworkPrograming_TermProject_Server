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

    CLONE(CItem)
public:
    void update() override;

public:
    int GetID() { return m_iID; }

    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }
    void SetID(int _iID) { m_iID = _iID; }

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CItem();
    ~CItem();
};