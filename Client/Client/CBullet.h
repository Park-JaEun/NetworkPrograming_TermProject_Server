#pragma once
#include "CObject.h"

class CBullet :
    public CObject
{
private:
    float   m_fDir;     // ����
    float   m_fSpeed;   // �ӵ�
    float   m_fDegree;  // ����
    float   m_fDeadTime;    // �ð�
    bool    m_bIsDown;  // �Ʒ��� �������� �Ѿ�����
    Vec2    m_vFirstPos;    // ó�� ��ġ
    Vec2    m_vPrevPos;     // ���� ��ġ

    CLONE(CBullet)
public:
    float GetDegree()               { return m_fDegree; }
    Vec2 GetFirstPos() { return m_vFirstPos; }

    void SetDir(int _iDir);
    void SetSpeed(float _fSpeed)    { m_fSpeed = _fSpeed; }
    void SetDegree(float _fDegree)  { m_fDegree = _fDegree; }
    void SetFirstPos(Vec2 _vPos) { m_vFirstPos = _vPos; }

    void PredictBulletPos();
    void InterpolatePos();

public:
    void OnCollision(CCollider* _pOther) override;
    void EnterCollision(CCollider* _pOther) override;
    void ExitCollision(CCollider* _pOther) override;

public:
    void update() override;
    void render(HDC _dc) override;

    void CreateMinjiBullet();
    void CreateDanielleBullet();
    void CreateHaerinBullet();
    void CreateHannieBullet();
    void CreateHyeinBullet();

public:
    void CreateAnimator(GROUP_TYPE _eType);
    void CreateAnimator(int id);

public:
    CBullet();
    CBullet(Vec2 vPos);
    ~CBullet();
};