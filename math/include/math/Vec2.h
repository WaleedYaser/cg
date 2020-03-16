#pragma once

#include <assert.h>
#include <math.h>

namespace math
{
	struct Vec2
	{
		float x, y;
	};

	static inline Vec2
	operator+(const Vec2 &a, const Vec2 &b)
	{
		return Vec2{ a.x + b.x, a.y + b.y };
	}

	inline static Vec2 &
	operator+=(Vec2 &a, const Vec2 &b)
	{
		a.x += b.x;
		a.y += b.y;
		return a;
	}

	static inline Vec2
	operator-(const Vec2 &a)
	{
		return Vec2 { -a.x, -a.y };
	}

	static inline Vec2
	operator-(const Vec2 &a, const Vec2 &b)
	{
		return Vec2{ a.x - b.x, a.y - b.y };
	}

	inline static Vec2 &
	operator-=(Vec2 &a, const Vec2 &b)
	{
		a.x -= b.x;
		a.y -= b.y;
		return a;
	}

	static inline Vec2
	operator *(const Vec2 &a, const Vec2 &b)
	{
		return Vec2{ a.x * b.x, a.y * b.y };
	}

	static inline Vec2 &
	operator*=(Vec2 &a, const Vec2 &b)
	{
		a.x *= b.x;
		a.y *= b.y;
		return a;
	}

	static inline Vec2
	operator*(const Vec2 &a, float f)
	{
		return Vec2{ a.x * f, a.y * f };
	}

	static inline Vec2
	operator*(float f, const Vec2 &a)
	{
		return a * f;
	}

	static inline Vec2 &
	operator*=(Vec2 &a, float f)
	{
		a.x *= f;
		a.y *= f;
		return a;
	}

	static inline Vec2
	operator/(const Vec2 &a, float f)
	{
		assert(f != 0 && "devision by zero!");
		return a * (1.0f / f);
	}

	static inline Vec2 &
	operator/=(Vec2 &a, float f)
	{
		assert(f != 0 && "devision by zero!");
		return a *= (1.0f / f);
	}

	static inline bool
	operator==(const Vec2 &a, const Vec2 &b)
	{
		return a.x == b.x && a.y == b.y;
	}

	static inline bool
	operator!=(const Vec2 &a, const Vec2 &b)
	{
		return !(a == b);
	}


	static inline float
	vec2_lenght(const Vec2 &a)
	{
		return sqrtf(a.x * a.x + a.y * a.y);
	}

	Vec2
	vec2_normalize(const Vec2 &a)
	{
		float len = vec2_lenght(a);
		assert(len > 0 && "zero vector lenght");
		return a / len;
	}
}