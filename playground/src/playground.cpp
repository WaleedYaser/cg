#include <math/Vec3.h>
#include <math/Mat4.h>

#include <stdio.h>

using namespace math;

int
main(int argc, char *argv[])
{
	Vec3 v1 = {1.0f, 2.0f, 3.0f};
	Vec3 v2 = {1.0f, 2.0f, 3.0f};
	Vec3 v3 = {1.0f, 2.0f, 3.0f};
	v3 += (v2 + v2 + v2);
	printf("(%f, %f, %f)", v3.x, v3.y, v3.z);

	Mat4 m = math::mat4_identity();

	printf("Hello, Playground!\n");
	return 0;
}