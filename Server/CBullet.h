#pragma once
#include "CObject.h"

class CBullet : public CObject {
private:
    float   m_fDir;         // 방향
    float   m_fSpeed;       // 속도
    float   m_fDegree;      // 각도
    bool    m_bIsDown;      // 아래로 내려가는 총알인지
    Vec2    m_vFirstPos;    // 처음 위치

    CLONE(CBullet)
public:
    float GetDegree() { return m_fDegree; }
    Vec2 GetFirstPos() { return m_vFirstPos; }

    void SetDir(int _iDir);
    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetDegree(float _fDegree) { m_fDegree = _fDegree; }
    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    void update() override;

public:
    CBullet();
    ~CBullet();
};

