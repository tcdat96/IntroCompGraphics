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
	Refraction* mRefraction;

	bool mHasMotion;
public:
	Sphere(mat4 xfm, Material material, Refraction refraction, bool hasMotion = false) :
		mMaterial(material), mHasMotion(hasMotion) {
		mXfm = xfm;
		mXfmInverse = glm::inverse(xfm);
		mRefraction = new Refraction(refraction);
	}

	~Sphere() {
		delete mRefraction;
	}

	mat4 getXfm() {
		return mXfm;
	}

	Material getMaterial() {
		return mMaterial;
	}

	Refraction* getRefraction() {
		return mRefraction;
	}

	void updateMotion() {
		if (mHasMotion) {
			mXfm = translate(mat4(1), vec3(0, 0.1, 0)) * mXfm;
			mXfmInverse = inverse(mXfm);
		}
	}

	bool isReflected() {
		return mMaterial.specular != dvec3(0);
	}

	dvec3 computeNormal(Intersection* ints) {
		dvec3 p = ints->uObj + ints->vObj * ints->t;
		dvec3 normal = transpose(mXfmInverse) * dvec4(p, 0);
		return normalize(normal);
	}

	Intersection* findIntersection(const Ray& ray) {
		dvec3 u = mXfmInverse * dvec4(ray.u, 1.0);
		dvec3 v = mXfmInverse * dvec4(ray.v, 0.0);

		double a = glm::dot(v, v);
		double b = 2 * glm::dot(u, v);
		double c = glm::dot(u, u) - 1;
		double delta = b * b - 4 * a * c;

		if (delta < 0) return nullptr;
		double root = sqrt(delta);
		double t = 0.5 * (-b - root) / a;
		if (t < 0) {
			t = 0.5 * (-b + root) / a;
		}
		return t < 0 ? nullptr : new Intersection(t, u, v, this);
	}
};

