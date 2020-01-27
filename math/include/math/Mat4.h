#pragma once

#include <stdint.h>

namespace math
{
	struct Mat4
	{
		float m[4][4];

		float *
		operator[] (uint8_t i) { return m[i]; }

		const float *
		operator[] (uint8_t i) const { return m[i]; }
	};

	inline static Mat4
	operator*(const Mat4 &A, const Mat4 &B)
	{
		Mat4 C;
		for (uint8_t i = 0; i < 4; ++i)
			for (uint8_t j = 0; j < 4; ++j)
				C[i][j] =
					A[i][0] * B[0][j] +
					A[i][1] * B[1][j] +
					A[i][2] * B[2][j] +
					A[i][3] * B[3][j];
		return C;
	}

	inline static Mat4 &
	operator*=(Mat4 &A, const Mat4 &B)
	{
		Mat4 C;
		for (uint8_t i = 0; i < 4; ++i)
			for (uint8_t j = 0; j < 4; ++j)
				C[i][j] =
					A[i][0] * B[0][j] +
					A[i][1] * B[1][j] +
					A[i][2] * B[2][j] +
					A[i][3] * B[3][j];
		A = C;
		return A;
	}

	inline static Mat4
	mat4_identity()
	{
		Mat4 A;
		A[0][0] = 1.0f, A[0][1] = 0.0f, A[0][2] = 0.0f, A[0][3] = 0.0f;
		A[1][0] = 0.0f, A[1][1] = 1.0f, A[1][2] = 0.0f, A[1][3] = 0.0f;
		A[2][0] = 0.0f, A[2][1] = 0.0f, A[2][2] = 1.0f, A[2][3] = 0.0f;
		A[3][0] = 0.0f, A[3][1] = 0.0f, A[3][2] = 0.0f, A[3][3] = 1.0f;
		return A;
	}

	inline static Mat4
	mat4_transpose(const Mat4 &A)
	{
		Mat4 T;
		for (uint8_t i = 0; i < 4; ++i)
			for (uint8_t j = 0; j < 4; ++j)
				T[i][j] = A[j][i];
		return T;
	}

	inline static Mat4
	mat4_inverse(const Mat4 &A)
	{
		assert(false && "Not implemented");
		return Mat4{}; 
	}
}