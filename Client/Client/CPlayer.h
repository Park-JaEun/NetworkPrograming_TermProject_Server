#pragma once
#include "CObject.h"

class CAnimator;

class CPlayer :
    public CObject
{
private:
    int             m_iHP;              // ÇÃ·¹ÀÌ¾î Ã¼·Â
    int             m_iLife;            // ÇÃ·¹ÀÌ¾î »ı¸í
    int             m_iBunnyCount;      // ÇÃ·¹ÀÌ¾î Åä³¢ ¼ö
    int             m_iCookieCount;     // ÇÃ·¹ÀÌ¾î ÄíÅ° ¼ö
    int             m_iKillCount;       // ÇÃ·¹ÀÌ¾î Å³ ¼ö
    float           m_fSpeed;           // ÇÃ·¹ÀÌ¾î ¼Óµµ
    float           m_fDieTime;         // ÇÃ·¹ÀÌ¾î »ç¸Á ½Ã°£
    float           m_fResurrectTime;   // ÇÃ·¹ÀÌ¾î ºÎÈ° ½Ã°£
    bool            m_bDir;             // ÇÃ·¹ÀÌ¾î ¹æÇâ
    bool			m_bIsGameOver;      // ÇÃ·¹ÀÌ¾î °ÔÀÓ¿À¹ö ¿©ºÎ
    PLAYER_STATE    m_eState;           // ÇÃ·¹ÀÌ¾î »óÅÂ
    CHARACTER_TYPE  m_eType;            // ÇÃ·¹ÀÌ¾î Å¸ÀÔ
    CAnimator*      m_EffectAnimator;   // ÀÌÆåÆ® ¾Ö´Ï¸ŞÀÌÅÍ

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
<<<<<<< HEAD

    void PredictPlayerPos();  // ÇÃ·¹ÀÌ¾î »çÀÌµå ½ºÅ©·Ñ¸µ ¿¹Ãø
    void InterpolatePos();    // ÇÃ·¹ÀÌ¾î »çÀÌµå ½ºÅ©·Ñ¸µ º¸°£
=======
  
>>>>>>> parent of 68322ae ([ê³µí†µ] í”Œë ˆì´ì–´ ìœ„ì¹˜ ì˜ˆì¸¡ê³¼ ë³´ê°„ ì¶”ê°€, ëª¨ë“  í´ë¼ì´ì–¸íŠ¸ í‚¤ì…ë ¥ ë™ê¸°í™” êµ¬í˜„)

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

