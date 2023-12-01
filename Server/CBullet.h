#pragma once
#include "CObject.h"

class CBullet : public CObject {
private:
    int     m_iID;          // ID
    int     m_iPlayerID;    // �߻��� �÷��̾� ID
    float   m_fDir;         // ����
    float   m_fSpeed;       // �ӵ�
    float   m_fDegree;      // ����
    bool    m_bIsDown;      // �Ʒ��� �������� �Ѿ�����
    Vec2    m_vFirstPos;    // ó�� ��ġ

    CLONE(CBullet)
public:
    float GetDir() { return m_fDir; }
    float GetDegree() { return m_fDegree; }
    Vec2 GetFirstPos() { return m_vFirstPos; }
    int GetID() { return m_iID; }
    int GetPlayerID() { return m_iPlayerID; }

    void SetDir(int _iDir);
    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetDegree(float _fDegree) { m_fDegree = _fDegree; }
    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }
    void SetID(int _iID) { m_iID = _iID; }
    void SetPlayerID(int _iID) { m_iPlayerID = _iID; }

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

