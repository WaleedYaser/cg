#include <math/Vec3.h>
#include <math/Mat4.h>

#include <stdio.h>

using namespace math;

int
main(int argc, char *argv[])
{
	Mat4 A = mat4_identity();
	Mat4 B = A;

	B[0][0] = 10;

	return 0;
}