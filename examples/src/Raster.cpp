#include <math/Vec2.h>
#include <math/Vec3.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace math;

typedef uint8_t RGB[3];

inline static float
_edge_function(const Vec3 &a, const Vec3 &b, const Vec3 &c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

int
main(int argc, char *argv[])
{
	Vec3 v0 = { 13.0f,  34.0f, 114.0f};
	Vec3 v1 = { 29.0f, -15.0f, 44.0f};
	Vec3 v2 = {-48.0f, -10.0f, 82.0f};

	Vec3 c0 = {1.0f, 0.0f, 0.0f};
	Vec3 c1 = {0.0f, 1.0f, 0.0f};
	Vec3 c2 = {0.0f, 0.0f, 1.0f};

	Vec2 st0 = {0.0f, 0.0f};
	Vec2 st1 = {1.0f, 0.0f};
	Vec2 st2 = {0.0f, 1.0f};

	constexpr uint32_t w = 512;
	constexpr uint32_t h = 512;

	// project tiangles onto screen
	v0.x /= v0.z, v0.y /= v0.z;
	v1.x /= v1.z, v1.y /= v1.z;
	v2.x /= v2.z, v2.y /= v2.z;

	// convert from screen space to NDC then raster
	v0.x = (1.0f + v0.x) * 0.5f * w, v0.y = (1.0f + v0.y) * 0.5f * h;
	v1.x = (1.0f + v1.x) * 0.5f * w, v1.y = (1.0f + v1.y) * 0.5f * h;
	v2.x = (1.0f + v2.x) * 0.5f * w, v2.y = (1.0f + v2.y) * 0.5f * h;

	// divide vertex-attribute by the vertex z-coords
	c0 /= v0.z, c1 /= v1.z, c2 /= v2.z;
	st0 /= v0.z, st1 /= v1.z, st2 /= v2.z;

	// pre-copmute 1 / over z
	v0.z = 1.0f / v0.z, v1.z = 1.0f / v1.z, v2.z = 1.0f / v2.z;

	RGB *framebuffer = (RGB *) malloc(w * h * sizeof(RGB));
	memset(framebuffer, 50, w * h * sizeof(RGB));

	float area = _edge_function(v0, v1, v2);

	for (uint32_t j = 0; j < h; ++j)
	{
		for (uint32_t i = 0; i < w; ++i)
		{
			Vec3 p = {i + 0.5f, h - j + 0.5f};
			float w0 = _edge_function(v1, v2, p);
			float w1 = _edge_function(v2, v0, p);
			float w2 = _edge_function(v0, v1, p);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 /= area;
				w1 /= area;
				w2 /= area;

				float r = w0 * c0.x + w1 * c1.x + w2 * c2.x;
				float g = w0 * c0.y + w1 * c1.y + w2 * c2.y;
				float b = w0 * c0.z + w1 * c1.z + w2 * c2.z;

				float s = w0 * st0.x + w1 * st1.x + w2 * st2.x;
				float t = w0 * st0.y + w1 * st1.y + w2 * st2.y;

				float z = 1.0f / (w0 *  v0.z + w1 * v1.z + w2 * v2.z);

				r *= z, g *= z, b *= z;
				s *= z, t *= z;

				constexpr int M = 10;
				// checkerboard pattern
				float p = (fmodf(s * M, 1.0f) > 0.5f) ^ (fmodf(t *M, 1.0f) < 0.5f);

				framebuffer[j * w + i][0] = uint8_t(r * p * 255.0f);
				framebuffer[j * w + i][1] = uint8_t(g * p * 255.0f);
				framebuffer[j * w + i][2] = uint8_t(b * p * 255.0f);
			}
		}
	}

	FILE *f = fopen("untitled.ppm", "wb");
	assert(f && "couldn't open file");
	fprintf(f, "P6\n%u %u\n255\n", w, h);
	fwrite(framebuffer, sizeof(RGB), w * h, f);
	fclose(f);

	free(framebuffer);
}