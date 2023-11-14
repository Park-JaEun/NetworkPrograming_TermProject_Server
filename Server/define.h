#pragma once

// ½Ì±ÛÅæ ¸ÅÅ©·Î
#define SINGLE(type) public: static type* GetInst() { static type mgr; return &mgr; }\
					 private: type(); ~type();

#define DT CTimer::GetInst()->GetDT()

#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key)	KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) 	KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key)	KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key)	KEY_CHECK(key, KEY_STATE::NONE)

#define ADDEVENT(_event) CEventMgr::GetInst()->AddEvent(_event)

#define CLONE(type) public: type* Clone() override { return new type(*this); }

#define DIR_RIGHT 0
#define DIR_LEFT  1
#define DIR_DOWN  2

#define MAX_PLAYER 3
#define BUFSIZE 512

#define MAX_FRAME 60
#define MAX_BULLET_DISTANCE 300.f

#define SERVERPORT 9000