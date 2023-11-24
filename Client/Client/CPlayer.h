#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // �÷��̾� ü��
    int             m_iLife;            // �÷��̾� ����
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
    CHARACTER_TYPE GetType() { return m_eType; }
    int GetLife() { return m_iLife; }

    void SetType(CHARACTER_TYPE _eType) { m_eType = _eType; }
    void SetState(PLAYER_STATE state) { m_eState = state; }
    void SetDir(bool dir) { m_bDir = dir; }

    void update() override;
    void render(HDC _dc) override;

private:
    void CreateBullet();

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

