#include <math/Vec3.h>
#include <math/Mat4.h>

#include <stdio.h>

using namespace math;

int
main(int argc, char *argv[])
{
	Vec3 a = Vec3{0.0f, 1.0f, 2.0f};

	Mat4 A = mat4_identity();
	Mat4 B = mat4_identity();
	Mat4 C = A * B;

	Mat4 D = mat4_new(
		0.707107, 0, -0.707107, 0,
		-0.331295, 0.883452, -0.331295, 0,
		0.624695, 0.468521, 0.624695, 0,
		4.000574, 3.00043, 4.000574, 1); 

	Mat4 D_inverted = mat4_inverse(D);

	Mat4 I = D * D_inverted;

	return 0;
}