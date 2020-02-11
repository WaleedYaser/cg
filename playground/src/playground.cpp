#include <stdio.h>

using Point = float[3];

int
main(int argc, char *argv[])
{

	Point corners[8] = {
		{1, -1, -5},
		{1, -1, -3},

		{1, 1, -5},
		{1, 1, -3},

		{-1, -1, -5},
		{-1, -1, -3},

		{-1, 1, -5,},
		{-1, 1, -3}
	};

	constexpr unsigned int image_width = 512, image_height = 512;

	for (int i = 0; i < 8; ++i)
	{
		// fivide x and y coordinates by the z coordinate to project the point on the canvas
		float x_proj = corners[i][0] / -corners[i][2];
		float y_proj = corners[i][1] / -corners[i][2];

		// map coordinates from [-1, 1] to [0, 1] (NDC)
		float x_proj_remap = (1 + x_proj) / 2.0f;
		float y_proj_remap = (1 + y_proj) / 2.0f;

		// image pixels (raster space)
		float x_proj_pix = x_proj_remap * image_width;
		float y_proj_pix = y_proj_remap * image_height;

		printf("projected corner: %d x: %5.2f y: %5.2f\n", i, x_proj_pix, y_proj_pix);
	}

	return 0;
}