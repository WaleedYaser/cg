#include <math/Vec3.h>
#include <math/Sphere.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <float.h>

#define M_PI 3.141592653589793
#define MAX_RAY_DEPTH 5

using namespace math;

inline static float
_blend(float a, float b, float t)
{
	return b * t + a * (1 - t);
}

inline static Vec3
_trace(Ray ray, Sphere spheres[], Material materials[], uint8_t count, uint8_t depth)
{
	float tnear = FLT_MAX;
	const Sphere *sphere = NULL;
	const Material *material = NULL;

	// find intersection of this ray with the sphere in the scene
	for (uint8_t i = 0; i < count; ++i)
	{
		Intersection_Result res = sphere_ray_intersect(spheres[i], ray);
		if (res.intersect)
		{
			if (res.t0 < 0.0f)
				res.t0 = res.t1;
			if (res.t0 < tnear)
			{
				tnear = res.t0;
				sphere = &spheres[i];
				material = &materials[i];
			}
		}
	}

	// if there's no intersection return white
	if (!sphere)
		return Vec3{1.0f, 1.0f, 1.0f};

	Vec3 surface_color = Vec3{0.0f, 0.0f, 0.0f}; // color of the ray/surface of the object intersected by the ray
	Vec3 phit = ray.origin + ray.dir * tnear; // point of intersection
	Vec3 nhit = phit - sphere->center; // normal at the intersectoin point
	nhit = vec3_normalize(nhit);

	// If the normal and the view direction are not opposite to each other
	// reverse the normal direction. That also means we are inside the sphere so set
	// the inside bool to true. Finally reverse the sign of IdotN whitch we want positive.
	float bias = 1e-4; // add some bias to the point from which we will be tracing
	bool inside = false;
	if (vec3_dot(ray.dir, nhit) > 0.0f)
	{
		nhit = -nhit;
		inside = true;
	}

	if ((material->transparency > 0.0f || material->reflection > 0.0f) && depth < MAX_RAY_DEPTH)
	{
		float facing_ratio = -vec3_dot(ray.dir, nhit);
		// change the mix value to tweak the effect
		float frensel_effect = _blend(powf(1 - facing_ratio, 3), 1.0f, 0.1f);

		// compute reflection direction (not need to normailze because all vectors are already normalized)
		Ray refl_ray;
		refl_ray.origin = phit + nhit * bias;
		refl_ray.dir = vec3_normalize(ray.dir - nhit * 2 * vec3_dot(ray.dir, nhit));
		Vec3 reflection = _trace(refl_ray, spheres, materials, count, depth + 1);
		Vec3 refraction = Vec3{0.0f, 0.0f, 0.0f};

		// if the sphere is also transparent compute refraction ray
		if (material->transparency > 0.0f)
		{
			float ior = 1.1f;
			float eta = (inside ? ior : 1.0f / ior);
			float cosi = -vec3_dot(nhit, ray.dir);
			float k = 1.0f - eta * eta * (1.0f - cosi * cosi);
			Ray refr_ray;
			refr_ray.origin = phit - nhit * bias;
			refr_ray.dir = vec3_normalize(ray.dir * eta + nhit * (eta * cosi - sqrt(k)));
			refraction = _trace(refr_ray, spheres, materials, count, depth + 1);
		}

		// the result is a mix of reflection and refraction (if the sphere is transparent)
		surface_color = (reflection * frensel_effect + refraction * (1 - frensel_effect) * material->transparency) * material->surface_color;
	}
	else
	{
		// it's a diffuse object, no need to raytrace any further
		for (uint8_t i = 0; i < count; ++i)
		{
			if (material[i].emission_color.x > 0.0f)
			{
				// this is a light
				float transmission = 1.0f;
				Vec3 light_dir = spheres[i].center - phit;
				light_dir = vec3_normalize(light_dir);
				for (uint8_t j = 0; j < count; ++j)
				{
					if (i != j)
					{
						Ray light_ray;
						light_ray.origin = phit + nhit * bias;
						light_ray.dir = light_dir;
						Intersection_Result res = sphere_ray_intersect(sphere[j], light_ray);
						if (res.intersect)
						{
							transmission = 0.0f;
							break;
						}
					}
				}

				float d = vec3_dot(nhit, light_dir);
				surface_color += material->surface_color * transmission * (d > 0.0f ? d : 0.0f) * material[i].emission_color;
			}
		}
	}

	return surface_color + material->emission_color;
}

inline static void
_render(Sphere spheres[], Material materials[], uint8_t count)
{
	constexpr uint16_t width = 640, height = 480;
	Vec3 *image = (Vec3 *)malloc(width * height * sizeof(Vec3));
	Vec3 *pixel = image;

	float inv_width = 1.0f / float(width), inv_height = 1.0f / float(height);
	float fov = 30.0f, aspect_ratio = float(width) / float(height);
	float angle = tanf(float(M_PI) * 0.5f * fov / 180.0f);

	// Trace rays
	for (uint16_t y = 0; y < height; ++y)
	{
		for (uint16_t x = 0; x < width; ++x, ++pixel)
		{
			float xx = (2 * ((x + 0.5f) * inv_width) - 1.0f) * angle * aspect_ratio;
			float yy = (1 - 2 * ((y + 0.5f) * inv_height)) * angle;
			Ray ray;
			ray.origin = Vec3{0.0f, 0.0f, 0.0f};
			ray.dir = vec3_normalize(Vec3{xx, yy, -1});
			*pixel = _trace(ray, spheres, materials, count, 0);
		}
	}

	// Same result to a PPM image
	FILE *f = fopen("untitled.ppm", "wb");
	assert(f && "couldn't open file");
	fprintf(f, "P6\n%u %u\n255\n", width, height);
	for (uint32_t i = 0; i < width * height; ++i)
	{
		uint8_t xx = uint8_t(image[i].x * 255);
		uint8_t yy = uint8_t(image[i].y * 255);
		uint8_t zz = uint8_t(image[i].z * 255);
		fwrite(&xx, sizeof(xx), 1, f);
		fwrite(&yy, sizeof(yy), 1, f);
		fwrite(&zz, sizeof(zz), 1, f);
	}
	fclose(f);
	free(image);
}

int
main(int argc, char *argv[])
{
	constexpr uint8_t count = 6;

	Sphere spheres[count];
	Material materials[count];

	spheres[0] = Sphere{ Vec3{ 0.0f, -10004.0f, -20.0f }, 10000.0f };
	spheres[1] = Sphere{ Vec3{ 0.0f,      0.0f, -20.0f },     4.0f };
	spheres[2] = Sphere{ Vec3{ 5.0f,     -1.0f, -15.0f },     2.0f };
	spheres[3] = Sphere{ Vec3{ 5.0f,      0.0f, -25.0f },     3.0f };
	spheres[4] = Sphere{ Vec3{-5.0f,      0.0f, -15.0f },     3.0f };
	// light sphere
	spheres[5] = Sphere{ Vec3{ 0.0f,     20.0f, -30.0f },     3.0f };

	materials[0].surface_color  = Vec3{ 0.2f, 0.2f, 0.2f };
	materials[0].emission_color = Vec3{ 0.0f, 0.0f, 0.0f };
	materials[0].transparency   = 0.0f;
	materials[0].reflection     = 0.0f;

	materials[1].surface_color  = Vec3{ 1.0f, 0.32f, 0.36f };
	materials[1].emission_color = Vec3{ 0.0f, 0.0f, 0.0f };
	materials[1].transparency   = 0.5f;
	materials[1].reflection     = 1.0f;

	materials[2].surface_color  = Vec3{ 0.99f, 0.76f, 0.46f };
	materials[2].emission_color = Vec3{ 0.0f, 0.0f, 0.0f };
	materials[2].transparency   = 0.0f;
	materials[2].reflection     = 1.0f;

	materials[3].surface_color  = Vec3{ 0.65f, 0.77f, 0.97f };
	materials[3].emission_color = Vec3{ 0.0f, 0.0f, 0.0f };
	materials[3].transparency   = 0.0f;
	materials[3].reflection     = 1.0f;

	materials[4].surface_color  = Vec3{ 0.9f, 0.9f, 0.9f };
	materials[4].emission_color = Vec3{ 0.0f, 0.0f, 0.0f };
	materials[4].transparency   = 0.0f;
	materials[4].reflection     = 1.0f;

	// light material
	materials[5].surface_color  = Vec3{ 0.0f, 0.0f, 0.0f };
	materials[5].emission_color = Vec3{ 3.0f, 3.0f, 3.0f };
	materials[5].transparency   = 0.0f;
	materials[5].reflection     = 0.0f;

	_render(spheres, materials, count);
}