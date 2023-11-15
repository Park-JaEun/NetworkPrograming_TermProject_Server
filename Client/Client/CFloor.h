#pragma once
#include "CObject.h"

class CFloor :
    public CObject
{
    CLONE(CFloor)
public:
    void update() override;
    void render(HDC _dc) override;

public:
    void CreateAnimator() override;

public:
    CFloor();
    ~CFloor();
};