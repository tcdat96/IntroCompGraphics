#pragma once

#include <stdio.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include "Entities.h"

class Sphere
{
private:
	Material mMaterial;
public:
	Sphere(Material material) {
		mMaterial = material;
	}

	Material getMaterial() {
		return mMaterial;
	}

	float findIntersection(const Ray& ray) {
		vec3 u = ray.u;
		vec3 v = ray.v;

		float a = glm::dot(v, v);
		float b = 2 * glm::dot(u, v);
		float c = glm::dot(u, u) - 1;
		float delta = b * b - 4 * a * c;


		if (delta < 0) return -1;
		float root = sqrt(delta);
		float t0 = 0.5f * (float(-b) - root) / a;
		if (t0 >= 0) return t0;
		float t1 = 0.5f * (float(-b) + root) / a;
		return t1 >= 0 ? t1 : -1;
	}
};

