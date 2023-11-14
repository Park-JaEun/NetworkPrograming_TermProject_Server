#pragma once
#include "CObject.h"

class CBullet :
    public CObject
{
private:
    float   m_fDir;     // 방향
    float   m_fSpeed;   // 속도
    float   m_fDegree;  // 각도
    bool    m_bIsDown;  // 아래로 내려가는 총알인지

    CLONE(CBullet)
public:
    void SetDir(int _iDir);
    void SetSpeed(float _fSpeed)    { m_fSpeed = _fSpeed; }
    void SetDegree(float _fDegree)  { m_fDegree = _fDegree; }

    float GetDegree()               { return m_fDegree; }

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    void update() override;
    void render(HDC _dc) override;

public:
    void CreateAnimator(GROUP_TYPE _eType);

public:
    CBullet();
    ~CBullet();
};

