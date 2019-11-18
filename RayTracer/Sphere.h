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
	mat4 mXfm;
	mat4 mXfmInverse;

	Material mMaterial;
	Refraction mRefraction;
public:
	Sphere(mat4 xfm, Material material, Refraction refraction): mMaterial(material), mRefraction(refraction) {
		mXfm = xfm;
		mXfmInverse = glm::inverse(xfm);
	}

	mat4 getXfm() {
		return mXfm;
	}

	Material getMaterial() {
		return mMaterial;
	}

	Refraction getRefraction() {
		return mRefraction;
	}

	bool isReflected() {
		return mMaterial.specular != dvec3(0);
	}

	dvec3 computeNormal(dvec3 point) {
		dvec3 center = getXfm() * vec4(0, 0, 0, 1);
		return glm::normalize(point - center);
	}

	double findIntersection(const Ray& ray) {
		dvec3 u = mXfmInverse * dvec4(ray.u, 1.0);
		dvec3 v = mXfmInverse * dvec4(ray.v, 0.0);

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

