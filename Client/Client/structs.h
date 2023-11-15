#pragma once

struct Vec2
{
	float x;
	float y;

public:
	void operator= (const POINT& _ptRhs) {
		x = (float)_ptRhs.x;
		y = (float)_ptRhs.y;
	}

	Vec2 operator+ (const Vec2& _vOther)
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

	Vec2 operator- (const Vec2& _vOther)
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}

	Vec2 operator* (const Vec2& _vOther)
	{
		return Vec2(x * _vOther.x, y * _vOther.y);
	}

	Vec2 operator/ (const Vec2& _vOther)
	{
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return Vec2(x / _vOther.x, y / _vOther.y);
	}

	Vec2 operator/ (float _fOther)
	{
		assert(!(0.f == _fOther));
		return Vec2(x / _fOther, y / _fOther);
	}

	Vec2 operator+ (const Vec2& _vOther) const
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

	Vec2 operator- (const Vec2& _vOther) const
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}

	Vec2 operator* (int _i) const
	{
		return Vec2(x * (float)_i, y * (float)_i);
	}

	Vec2 operator* (const Vec2& _vOther) const
	{
		return Vec2(x * _vOther.x, y * _vOther.y);
	}

	Vec2 operator/ (const Vec2& _vOther) const
	{
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return Vec2(x / _vOther.x, y / _vOther.y);
	}

	Vec2 operator+= (const Vec2& _vOther) const
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

public:
	Vec2() : x(0.f), y(0.f)
	{}

	Vec2(float _x, float _y) : x(_x), y(_y)
	{}

	Vec2(int _x, int _y) : x((float)_x), y((float)_y)
	{}

	Vec2(LONG _x, LONG _y) : x((float)_x), y((float)_y)
	{}

	Vec2(UINT _x, UINT _y) : x((float)_x), y((float)_y)
	{}

	Vec2(const POINT& _pt) : x((float)_pt.x), y((float)_pt.y)
	{}
};