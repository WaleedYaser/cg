#include <math/Mat4.h>
#include <math/Vec3.h>
#include <math/Vec4.h>

#define _USE_MATH_DEFINES
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

using namespace math;

const Vec3 verts[146] = {
    {  -2.5703f,   0.78053f,  -2.4e-05f}, { -0.89264f,  0.022582f,  0.018577f},
    {   1.6878f, -0.017131f,  0.022032f}, {   3.4659f,  0.025667f,  0.018577f},
    {  -2.5703f,   0.78969f, -0.001202f}, { -0.89264f,   0.25121f,   0.93573f},
    {   1.6878f,   0.25121f,    1.1097f}, {   3.5031f,   0.25293f,   0.93573f},
    {  -2.5703f,    1.0558f, -0.001347f}, { -0.89264f,    1.0558f,    1.0487f},
    {   1.6878f,    1.0558f,    1.2437f}, {   3.6342f,    1.0527f,    1.0487f},
    {  -2.5703f,    1.0558f,       0.0f}, { -0.89264f,    1.0558f,       0.0f},
    {   1.6878f,    1.0558f,       0.0f}, {   3.6342f,    1.0527f,       0.0f},
    {  -2.5703f,    1.0558f,  0.001347f}, { -0.89264f,    1.0558f,   -1.0487f},
    {   1.6878f,    1.0558f,   -1.2437f}, {   3.6342f,    1.0527f,   -1.0487f},
    {  -2.5703f,   0.78969f,  0.001202f}, { -0.89264f,   0.25121f,  -0.93573f},
    {   1.6878f,   0.25121f,   -1.1097f}, {   3.5031f,   0.25293f,  -0.93573f},
    {   3.5031f,   0.25293f,       0.0f}, {  -2.5703f,   0.78969f,       0.0f},
    {   1.1091f,    1.2179f,       0.0f}, {    1.145f,     6.617f,       0.0f},
    {   4.0878f,    1.2383f,       0.0f}, {  -2.5693f,    1.1771f, -0.081683f},
    {  0.98353f,    6.4948f, -0.081683f}, { -0.72112f,    1.1364f, -0.081683f},
    {   0.9297f,     6.454f,       0.0f}, {  -0.7929f,     1.279f,       0.0f},
    {  0.91176f,    1.2994f,       0.0f}
};

const uint32_t tri_count = 51;

const uint32_t tris[tri_count * 3] = {
     4,   0,   5,   0,   1,   5,   1,   2,   5,   5,   2,   6,   3,   7,   2,
     2,   7,   6,   5,   9,   4,   4,   9,   8,   5,   6,   9,   9,   6,  10,
     7,  11,   6,   6,  11,  10,   9,  13,   8,   8,  13,  12,  10,  14,   9,
     9,  14,  13,  10,  11,  14,  14,  11,  15,  17,  16,  13,  12,  13,  16,
    13,  14,  17,  17,  14,  18,  15,  19,  14,  14,  19,  18,  16,  17,  20,
    20,  17,  21,  18,  22,  17,  17,  22,  21,  18,  19,  22,  22,  19,  23,
    20,  21,   0,  21,   1,   0,  22,   2,  21,  21,   2,   1,  22,  23,   2,
     2,  23,   3,   3,  23,  24,   3,  24,   7,  24,  23,  15,  15,  23,  19,
    24,  15,   7,   7,  15,  11,   0,  25,  20,   0,   4,  25,  20,  25,  16,
    16,  25,  12,  25,   4,  12,  12,   4,   8,  26,  27,  28,  29,  30,  31,
    32,  34,  33
};

float focal_length = 55.0f; // in mm

// 35 Full Aperture in inches
float film_aperture_width = 0.980f;
float film_aperture_height = 0.735f;
static const float inch_to_mm = 25.4f;
float near_clipping_plane = 0.1f;
float far_clipping_plane = 1000.0f;

// image resolution in pixel
uint32_t image_width = 640;
uint32_t image_height = 480;

enum Fit_Resolution_Gate
{
	FIT_RESOLUTION_GATE_FILL,
	FIT_RESOLUTION_GATE_OVERSCAN
};

Fit_Resolution_Gate fit_film = FIT_RESOLUTION_GATE_OVERSCAN;

struct Pixel_Coord_Result
{
	Vec3 coord;
	bool visible;
};

inline static Pixel_Coord_Result
compute_pixel_coord(
	const Vec3 &p_world, const Mat4 &world_to_camera,
	float bottom, float left, float top, float right, float near,
	uint32_t image_width, uint32_t image_height)
{
	Vec4 p_cam =  Vec4{p_world.x, p_world.y, p_world.z, 1.0f} * world_to_camera;
	Vec3 p_screen = Vec3{p_cam.x / -p_cam.z * near, p_cam.y / -p_cam.z * near, 0.0f};

	Vec3 p_ndc = Vec3{
		(p_screen.x + right) / (2.0f * right),
		(p_screen.y + top) / (2.0f * top),
		0.0f};

	Vec3 p_raster = Vec3 {p_ndc.x * image_width, (1.0f - p_ndc.y) * image_height, 0.0f};

	bool visible = true;
	if (p_screen.x < left || p_screen.x > right || p_screen.y < bottom || p_screen.y > top)
		visible = false;

	return Pixel_Coord_Result{p_raster, visible};
}

int
main(int argc, char *argv[])
{
	float film_aspect_ratio = film_aperture_width / film_aperture_height;
	float device_ascpect_ratio = float(image_width) / float(image_height);

	float top = ((film_aperture_height * inch_to_mm / 2.0f) / focal_length) * near_clipping_plane;
	float right = ((film_aperture_width * inch_to_mm / 2.0f) / focal_length) * near_clipping_plane;

	float x_scale = 1.0f;
	float y_scale = 1.0f;

	switch (fit_film)
	{
	default:
	case FIT_RESOLUTION_GATE_FILL:
		if (film_aspect_ratio > device_ascpect_ratio)
		{
			x_scale = device_ascpect_ratio / film_aspect_ratio;
		}
		else
		{
			y_scale = film_aspect_ratio / device_ascpect_ratio;
		}
		break;
	case FIT_RESOLUTION_GATE_OVERSCAN:
		if (film_aspect_ratio > device_ascpect_ratio)
		{
			y_scale = film_aspect_ratio / device_ascpect_ratio;
		}
		else
		{
			x_scale = device_ascpect_ratio / film_aspect_ratio;
		}
		break;
	}

	top *= y_scale;
	right *= x_scale;
	float bottom = -top;
	float left = -right;

	printf("Screen window coordinates: %0.2f %0.2f %0.2f %0.2f\n", bottom, left, top, right);
	printf("Film Aspect Ratio: %0.2f\nDevice Aspect Ratio: %0.2f\n", film_aspect_ratio, device_ascpect_ratio);
	printf("Angle of view: %0.2f deg\n", 2.0f * atan((film_aperture_width * inch_to_mm / 2.0f) / focal_length) * 180 / M_PI);

	FILE *file = fopen("untitled.svg", "w");
	assert(file && "couldn't open file");
	fprintf(file, "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" height=\"%d\" width=\"%d\">\n", image_height, image_width);

	Mat4 camera_to_world = mat4_new(
		-0.95424f, 0.0f, 0.299041f, 0.0f,
		0.0861242f, 0.95763f, 0.274823f, 0.0f,
		-0.28637f, 0.288002f, -0.913809f, 0.0f,
		-3.734612f, 7.610426f, -14.152769f, 1.0f);

	Mat4 world_to_camera = mat4_inverse(camera_to_world);

	for (uint32_t i = 0; i < tri_count; ++i)
	{
		const Vec3 &v0_world = verts[tris[i * 3]];
		const Vec3 &v1_world = verts[tris[i * 3 + 1]];
		const Vec3 &v2_world = verts[tris[i * 3 + 2]];

		auto [v0_raster, v0_visible] = compute_pixel_coord(
			v0_world, world_to_camera,
			bottom, left, top, right, near_clipping_plane,
			image_width, image_height);

		auto [v1_raster, v1_visible] = compute_pixel_coord(
			v1_world, world_to_camera,
			bottom, left, top, right, near_clipping_plane,
			image_width, image_height);

		auto [v2_raster, v2_visible] = compute_pixel_coord(
			v2_world, world_to_camera,
			bottom, left, top, right, near_clipping_plane,
			image_width, image_height);

		int val = (v0_visible && v1_visible && v2_visible) ? 0 : 255;

		fprintf(
			file,
			"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(%d,0,0);stroke-width:1\" />\n",
			int(v0_raster.x), int(v0_raster.y), int(v1_raster.x), int(v1_raster.y), val);

		fprintf(
			file,
			"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(%d,0,0);stroke-width:1\" />\n",
			int(v1_raster.x), int(v1_raster.y), int(v2_raster.x), int(v2_raster.y), val);

		fprintf(
			file,
			"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(%d,0,0);stroke-width:1\" />\n",
			int(v2_raster.x), int(v2_raster.y), int(v0_raster.x), int(v0_raster.y), val);
	}

	fprintf(file, "</svg>\n");

	fclose(file);

	return 0;
}