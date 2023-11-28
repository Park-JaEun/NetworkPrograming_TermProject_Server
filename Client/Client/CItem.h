#pragma once
#include "CObject.h"

class CItem :
    public CObject
{
private:
    Vec2            m_vFirstPos;
    float           m_fSpeed;
    float           m_fMaxDistance;
    bool            m_bDir;

    CLONE(CItem)
public:
    void update() override;
    void render(HDC _dc) override;

public:
    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CItem();
    ~CItem();
};