#pragma once

#include <math.h>
#include <assert.h>

namespace math
{
	// for point make sure to set w to 1 and divide by it if it's not
	// for vector make sure to set w to 0
	struct Vec4
	{
		float x, y, z, w;
	};

	static inline Vec4
	operator+(const Vec4 &a, const Vec4 &b)
	{
		return Vec4{
			a.x + b.x,
			a.y + b.y,
			a.z + b.z
			a.w + b.w
		};
	}

	inline static Vec4 &
	operator+=(Vec4 &a, const Vec4 &b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;
		return a;
	}

	static inline Vec4
	operator-(const Vec4 &a)
	{
		return Vec4 {
			-a.x,
			-a.y,
			-a.z,
			-a.w
		};
	}

	static inline Vec4
	operator-(const Vec4 &a, const Vec4 &b)
	{
		return Vec4{
			a.x - b.x,
			a.y - b.y,
			a.z - b.z,
			a.w - b.w
		};
	}

	inline static Vec4 &
	operator-=(Vec4 &a, const Vec4 &b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		a.w -= b.w;
		return a;
	}

	static inline Vec4
	operator*(const Vec4 &a, float f)
	{
		return Vec4{
			a.x * f,
			a.y * f,
			a.z * f,
			a.w * f
		};
	}

	static inline Vec4
	operator*(float f, const Vec4 &a)
	{
		return a * f;
	}

	static inline Vec4 &
	operator*=(Vec4 &a, float f)
	{
		a.x *= f;
		a.y *= f;
		a.z *= f;
		a.w *= f;
		return a;
	}

	static inline Vec4
	operator *(const Vec4 &a, const Mat4 &B)
	{
		return Vec4 {
			a.x * B[0][0] + a.y * B[1][0] + a.z * B[2][0] + a.w * B[3][0],
			a.x * B[0][1] + a.y * B[1][1] + a.z * B[2][1] + a.w * B[3][1],
			a.x * B[0][2] + a.y * B[1][2] + a.z * B[2][2] + a.w * B[3][2],
			a.x * B[0][3] + a.y * B[1][3] + a.z * B[2][3] + a.w * B[3][3]
		};
	}

	static inline Vec4 &
	operator*=(Vec4 &a, const Mat4 &B)
	{
		Vec4 c = {
			a.x * B[0][0] + a.y * B[1][0] + a.z * B[2][0] + a.w * B[3][0],
			a.x * B[0][1] + a.y * B[1][1] + a.z * B[2][1] + a.w * B[3][1],
			a.x * B[0][2] + a.y * B[1][2] + a.z * B[2][2] + a.w * B[3][2],
			a.x * B[0][3] + a.y * B[1][3] + a.z * B[2][3] + a.w * B[3][3]
		};

		a = c;
		return a;
	}

	static inline Vec4
	operator/(const Vec4 &a, float f)
	{
		assert(f != 0 && "devision by zero!");
		return a * (1.0f / f);
	}

	static inline Vec4 &
	operator/=(Vec4 &a, float f)
	{
		assert(f != 0 && "devision by zero!");
		return a *= (1.0f / f);
	}

	static inline bool
	operator==(const Vec4 &a, const Vec4 &b)
	{
		return
			a.x == b.x &&
			a.y == b.y &&
			a.z == b.z &&
			a.w == b.w;
	}

	static inline bool
	operator!=(const Vec4 &a, const Vec4 &b)
	{
		return !(a == b);
	}


	static inline float
	vec4_lenght(const Vec4 &a)
	{
		return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	}

	Vec4
	vec4_normalize(const Vec4 &a)
	{
		float len = vec4_lenght(a);
		assert(len > 0 && "zero vector lenght");
		return a / len;
	}

	float
	vec4_dot(const Vec4 &a, const Vec4 &b)
	{
		return
			a.x * b.x +
			a.y * b.y +
			a.z * b.z +
			a.w * b.w;
	}

	Vec4
	vec4_cross(const Vec4 &a, const Vec4 &b)
	{
		return Vec4 {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

}