#pragma once

// 매크로 함수
// 함수처럼 사용하면 해당 부분을 치환해준다.
// 반환을 하는 게 아니고, 치환한다는 것을 알고, 주의해야한다.
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