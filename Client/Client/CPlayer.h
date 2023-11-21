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
    float           m_fDieTime;         // �÷��̾� �ִ� �̵� �Ÿ�
    bool            m_bDir;             // �÷��̾� ����
    PLAYER_STATE    m_eState;           // �÷��̾� ����
    CHARACTER_TYPE  m_eType;            // �÷��̾� Ÿ��
    CAnimator*      m_EffectAnimator;   // ����Ʈ �ִϸ�����

    CLONE(CPlayer)
public:
    CHARACTER_TYPE GetType() { return m_eType; }
    void SetType(CHARACTER_TYPE _eType) { m_eType = _eType; }

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

