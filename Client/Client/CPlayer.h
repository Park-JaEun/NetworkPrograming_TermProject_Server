#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // �÷��̾� ü��
    float           m_fSpeed;           // �÷��̾� �ӵ�
    float           m_fDieTime;         // �÷��̾� �ִ� �̵� �Ÿ�
    bool            m_bDir;             // �÷��̾� ����
    PLAYER_STATE    m_eState;           // �÷��̾� ����
    CAnimator*      m_EffectAnimator;   // ����Ʈ �ִϸ�����

    CLONE(CPlayer)
public:
    void update() override;
    void render(HDC _dc) override;

private:
    void CreateBullet();

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

