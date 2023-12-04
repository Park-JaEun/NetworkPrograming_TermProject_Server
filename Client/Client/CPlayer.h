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
    int             m_iCookieCount;     // �÷��̾� ��Ű ��
    int             m_iKillCount;       // �÷��̾� ų ��
    float           m_fSpeed;           // �÷��̾� �ӵ�
    float           m_fDieTime;         // �÷��̾� ��� �ð�
    float           m_fResurrectTime;   // �÷��̾� ��Ȱ �ð�
    bool            m_bDir;             // �÷��̾� ����
    bool			m_bIsGameOver;      // �÷��̾� ���ӿ��� ����
    PLAYER_STATE    m_eState;           // �÷��̾� ����
    CHARACTER_TYPE  m_eType;            // �÷��̾� Ÿ��
    CAnimator*      m_EffectAnimator;   // ����Ʈ �ִϸ�����

    CLONE(CPlayer)
public:
    const CHARACTER_TYPE GetType()    { return m_eType; }
    const int GetLife()               { return m_iLife; }
    const int GetBunnyCount()         { return m_iBunnyCount; }
    const int GetCookieCount()        { return m_iCookieCount; }
    const int GetKillCount()          { return m_iKillCount; }
    const bool GetIsGameOver()        { return m_bIsGameOver; }

    void SetType(CHARACTER_TYPE _eType) { m_eType = _eType; }
    void SetState(PLAYER_STATE state)   { m_eState = state; }
    void SetDir(bool dir)               { m_bDir = dir; }
    void SetLife(int life)              { m_iLife = life; }
    void SetHP(int hp)                  { m_iHP = hp; }
    void SetBunnyCount(int count)       { m_iBunnyCount = count; }
    void SetCookieCount(int count)      { m_iCookieCount = count; }
    void SetKillCount(int count)        { m_iKillCount = count; }
    void SetIsGameOver(bool isGameOver) { m_bIsGameOver = isGameOver; }

    void PlayShootingEffect();

    void update() override;
    void render(HDC _dc) override;

private:
    void CreateBullet(int id);

    void CreateMinji();
    void CreateHannie();
    void CreateDanielle();
    void CreateHaerin();
    void CreateHyein();

    void PlayAnimation();
  

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;
    
public:
    void CreateAnimator() override;

public:
    CPlayer();
    ~CPlayer();
};

