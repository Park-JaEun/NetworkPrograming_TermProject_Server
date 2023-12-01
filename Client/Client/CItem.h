#pragma once
#include "CObject.h"

class CItem :
    public CObject
{
private:
    Vec2            m_vFirstPos;
    Vec2            m_vPrevPos;
    float           m_fSpeed;
    float           m_fMaxDistance;
    float           m_fTime;
    bool            m_bDir;
    bool            m_bIsExist;
    ITEM_TYPE	    m_eType;

    CLONE(CItem)
public:
    void update() override;
    void render(HDC _dc) override;

public:
    ITEM_TYPE GetType() { return m_eType; }

    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }
    void SetType(ITEM_TYPE _eType) { m_eType = _eType; }

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CItem();
    ~CItem();
};