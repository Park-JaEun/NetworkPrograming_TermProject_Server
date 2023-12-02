#pragma once
#include "CObject.h"

class CBullet : public CObject {
private:
    int     m_iID;          // ID
    int     m_iPlayerID;    // 발사한 플레이어 ID
    float   m_fDir;         // 방향
    float   m_fSpeed;       // 속도
    float   m_fDegree;      // 각도
    bool    m_bIsDown;      // 아래로 내려가는 총알인지
    Vec2    m_vFirstPos;    // 처음 위치
    GROUP_TYPE m_eGroupType;// 그룹 타입

    CLONE(CBullet)
public:
    const float GetDir()            { return m_fDir; }
    const float GetDegree()         { return m_fDegree; }
    const Vec2 GetFirstPos()        { return m_vFirstPos; }
    const int GetID()               { return m_iID; }
    const int GetPlayerID()         { return m_iPlayerID; }
    const GROUP_TYPE GetGroupType() { return m_eGroupType; }

    void SetDir(int _iDir);
    void SetSpeed(float _fSpeed)                { m_fSpeed = _fSpeed; }
    void SetDegree(float _fDegree)              { m_fDegree = _fDegree; }
    void SetFirstPos(Vec2 _vPos)                { m_vFirstPos = _vPos; }
    void SetID(int _iID)                        { m_iID = _iID; }
    void SetPlayerID(int _iID)                  { m_iPlayerID = _iID; }
    void SetGroupType(GROUP_TYPE _eGroupType)   { m_eGroupType = _eGroupType; }

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

