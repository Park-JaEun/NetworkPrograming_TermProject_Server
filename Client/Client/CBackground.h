#pragma once
#include "CObject.h"

class CTexture;

class CBackground :
    public CObject
{
    CLONE(CBackground)
public:
    void update() override;
    void render(HDC _dc) override;

public:
    void CreateAnimator() override;

public:
    CBackground();
    ~CBackground();
};


