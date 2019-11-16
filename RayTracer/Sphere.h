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
		mMaterial = yRubber;
	}

	Material getMaterial() {
		return mMaterial;
	}

	double findIntersection(const Ray& ray) {
		dvec3 u = ray.u;
		dvec3 v = ray.v;

		double a = glm::dot(v, v);
		double b = 2 * glm::dot(u, v);
		double c = glm::dot(u, u) - 1;
		double delta = b * b - 4 * a * c;

		if (delta < 0) return -1;
		double root = sqrt(delta);
		double t0 = 0.5 * (-b - root) / a;
		if (t0 >= 0) return t0;
		double t1 = 0.5 * (-b + root) / a;
		return t1 >= 0 ? t1 : -1;
	}
};

