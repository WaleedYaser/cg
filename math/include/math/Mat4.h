#pragma once

#include <assert.h>
#include <math.h>
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
	mat4_new(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		Mat4 A;
		A[0][0] = m00, A[0][1] = m01, A[0][2] = m02, A[0][3] = m03;
		A[1][0] = m10, A[1][1] = m11, A[1][2] = m12, A[1][3] = m13;
		A[2][0] = m20, A[2][1] = m21, A[2][2] = m22, A[2][3] = m23;
		A[3][0] = m30, A[3][1] = m31, A[3][2] = m32, A[3][3] = m33;
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
		// clone A
		Mat4 M = A;
		// result
		Mat4 R = mat4_identity();
		for (uint8_t column = 0; column < 4; ++column)
		{
			// swap row in case out pivot point is not working
			if (M[column][column] == 0.0f)
			{
				uint8_t row_max = column;
				for (uint8_t row = 0; row < 4; ++row)
					if (fabsf(M[row][column]) > fabsf(M[row_max][column]))
						row_max = row;
				if (row_max = column)
				{
					// TODO: singular matrix
					assert(false && "singular matrix");
				}
				// Swap rows
				else
				{
					for (uint8_t j = 0; j < 4; ++j)
					{
						float tmp = M[column][j];
						M[column][j] = M[row_max][j];
						M[row_max][j] = tmp;

						tmp = R[column][j];
						R[column][j] = R[row_max][j];
						R[row_max][j] = tmp;
					}
				}
			}

			// Set each row in the column to 0
			for (uint8_t row = 0; row < 4; ++row)
			{
				if (row != column)
				{
					float coeff = M[row][column] / M[column][column];
					if (coeff != 0.0f)
					{
						for (uint8_t j = 0; j < 4; ++j)
						{
							M[row][j] -= coeff * M[column][j];
							R[row][j] -= coeff * R[column][j];
						}
						// Set the element to 0 for safety
						M[row][column] = 0.0f;
					}
				}
			}
		}

		// Set each element of the diagonal to 1
		for (uint8_t row = 0; row < 4; ++row)
		{
			for (uint8_t column = 0; column < 4; ++column)
			{
				R[row][column] /= M[row][row];
			}
		}
		return R;
	}
}