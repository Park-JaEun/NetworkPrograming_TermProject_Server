#pragma once
#include "CObject.h"
class CUI :
    public CObject
{
private:
	CLONE(CUI)
	bool m_bOnMouse;
	bool m_bMousePush;

public:
	void SetOnMouse(bool _bOnMouse) { m_bOnMouse = _bOnMouse; }
	void SetMousePush(bool _bMousePush) { m_bMousePush = _bMousePush; }

	bool GetOnMouse() { return m_bOnMouse; }
	bool GetMousePush() { return m_bMousePush; }

public:
    void update() override;
	void render(HDC _dc) override;

public:
	void CreateAnimator() override;

public:
    CUI();
	~CUI();
};

