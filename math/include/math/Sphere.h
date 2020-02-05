// TODO(Waleed): move to geo library and separate material
#pragma once

#include <math/Vec3.h>

#include <float.h>

namespace math
{
	struct Sphere
	{
		Vec3 center;
		float radius;
	};

	struct Material
	{
		Vec3 surface_color;
		Vec3 emission_color;
		float transparency;
		float reflection;
	};

	struct Ray
	{
		Vec3 origin;
		Vec3 dir;
	};

	struct Intersection_Result
	{
		bool intersect;
		float t0;
		float t1;
	};

	Intersection_Result
	sphere_ray_intersect(const Sphere &self, const Ray &ray)
	{
		Intersection_Result res;
		res.intersect = false;
		res.t0 = FLT_MAX;
		res.t1 = FLT_MAX;

		Vec3 l = self.center - ray.origin;
		float tca = vec3_dot(l, ray.dir);
		if (tca < 0) 
			return res;

		float radius2 = self.radius * self.radius;
		float d2 = vec3_dot(l, l) - tca * tca;
		if (d2 > radius2)
			return res;

		float thc = sqrtf(radius2 - d2);

		res.intersect = true;
		res.t0 = tca - thc;
		res.t1 = tca + thc;
		return res;
	}
}