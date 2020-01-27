#pragma once

#include "math/Vec3.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace math
{
	struct Polar
	{
		float theta; // angle in radian between vector and vertical axis [0: PI]
		float phi;	 // angle in radian between vector projection on horizontal plan and right axis [0: 2PI]
		float r;	 // vector lenght
	};

	Polar
	polar_from_vec3(const Vec3 &a)
	{
		Vec3 n = normalize(a);

		float theta = acos(n.z);
		float p = atan2(n.y, n.x);
		float phi = (p < 0) ? p + 2 * M_PI : p;

		return Polar {
			theta,
			phi,
			lenght(a)
		};
	}

	Vec3
	polar_to_vec3(const Polar &self)
	{
		float cos_phi   = cosf(self.phi);
		float sin_phi   = sinf(self.phi);
		float cos_theta = cosf(self.theta);
		float sin_theta = sinf(self.theta);

		return Vec3 {
			self.r * cos_phi * sin_theta,
			self.r * sin_phi * sin_theta,
			self.r * cos_theta
		};
	}
}

#undef _USE_MATH_DEFINES