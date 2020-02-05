#pragma once

#include <math.h>
#include <assert.h>

namespace math
{
	struct Vec3
	{
		float x, y, z;
	};

	static inline Vec3
	operator+(const Vec3 &a, const Vec3 &b)
	{
		return Vec3{
			a.x + b.x,
			a.y + b.y,
			a.z + b.z
		};
	}

	inline static Vec3 &
	operator+=(Vec3 &a, const Vec3 &b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		return a;
	}

	static inline Vec3
	operator-(const Vec3 &a)
	{
		return Vec3 {
			-a.x,
			-a.y,
			-a.z
		};
	}

	static inline Vec3
	operator-(const Vec3 &a, const Vec3 &b)
	{
		return Vec3{
			a.x - b.x,
			a.y - b.y,
			a.z - b.z
		};
	}

	inline static Vec3 &
	operator-=(Vec3 &a, const Vec3 &b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		return a;
	}

	static inline Vec3
	operator *(const Vec3 &a, const Vec3 &b)
	{
		return Vec3{
			a.x * b.x,
			a.y * b.y,
			a.z * b.z
		};
	}

	static inline Vec3 &
	operator*=(Vec3 &a, const Vec3 &b)
	{
		a.x *= b.x;
		a.y *= b.y;
		a.z *= b.z;
		return a;
	}

	static inline Vec3
	operator*(const Vec3 &a, float f)
	{
		return Vec3{
			a.x * f,
			a.y * f,
			a.z * f
		};
	}

	static inline Vec3
	operator*(float f, const Vec3 &a)
	{
		return a * f;
	}

	static inline Vec3 &
	operator*=(Vec3 &a, float f)
	{
		a.x *= f;
		a.y *= f;
		a.z *= f;
		return a;
	}

	static inline Vec3
	operator/(const Vec3 &a, float f)
	{
		assert(f != 0 && "devision by zero!");
		return a * (1.0f / f);
	}

	static inline Vec3 &
	operator/=(Vec3 &a, float f)
	{
		assert(f != 0 && "devision by zero!");
		return a *= (1.0f / f);
	}

	static inline bool
	operator==(const Vec3 &a, const Vec3 &b)
	{
		return
			a.x == b.x &&
			a.y == b.y &&
			a.z == b.z;
	}

	static inline bool
	operator!=(const Vec3 &a, const Vec3 &b)
	{
		return !(a == b);
	}


	static inline float
	vec3_lenght(const Vec3 &a)
	{
		return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	Vec3
	vec3_normalize(const Vec3 &a)
	{
		float len = vec3_lenght(a);
		assert(len > 0 && "zero vector lenght");
		return a / len;
	}

	float
	vec3_dot(const Vec3 &a, const Vec3 &b)
	{
		return
			a.x * b.x +
			a.y * b.y +
			a.z * b.z;
	}

	Vec3
	vec3_cross(const Vec3 &a, const Vec3 &b)
	{
		return Vec3 {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}
}