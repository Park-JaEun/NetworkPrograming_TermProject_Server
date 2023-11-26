#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // �÷��̾� ü��
    int             m_iLife;            // �÷��̾� ����
    int             m_iBunnyCount;      // �÷��̾� �䳢 ��
    int             m_iCookieCount;     // �÷��̾� �䳢 ��
    int             m_iKillCount;       // �÷��̾� ų ��
    float           m_fSpeed;           // �÷��̾� �ӵ�
    float           m_fDieTime;         // �÷��̾� ��� �ð�
    float           m_fResurrectTime;   // �÷��̾� ��Ȱ �ð�
    bool            m_bDir;             // �÷��̾� ����
    bool			m_bIsGameOver;      // �÷��̾� ���ӿ��� ����
    PLAYER_STATE    m_eState;           // �÷��̾� ����
    CHARACTER_TYPE  m_eType;            // �÷��̾� Ÿ��

    CLONE(CPlayer)
public:
    float GetSpeed()                      {      return m_fSpeed;    }
    bool GetDir()                       {        return m_bDir; }
    PLAYER_STATE GetState()             { return m_eState; }

    void SetState(PLAYER_STATE state)    { m_eState = state; }
    void SetDir(bool dir)               { m_bDir = dir; }

    CHARACTER_TYPE GetType() { return m_eType; }
    int GetLife() { return m_iLife; }
    int GetBunnyCount() { return m_iBunnyCount; }
    int GetCookieCount() { return m_iCookieCount; }
    int GetKillCount() { return m_iKillCount; }

    void SetType(CHARACTER_TYPE _eType) { m_eType = _eType; }

    void PlusBunnyCount() { ++m_iBunnyCount; }
    void PlusCookieCount() { ++m_iCookieCount; }
    void PlusKillCount() { ++m_iKillCount; }

    void update() override;

private:
    void CreateBullet();

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    CPlayer();
    ~CPlayer();
};