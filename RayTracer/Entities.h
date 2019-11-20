#pragma once

#include "Sphere.h"

class Sphere;

const double AIR_COEFFICENT = 1.0002926;

struct Pixel {
	int r, g, b;
};

struct Ray {
	dvec3 u;
	dvec3 v;
	short depth = 0;
	Ray(dvec3 u) : u(u) {}
	Ray(dvec3 u, dvec3 v, int depth = 0) : u(u), v(v), depth(depth) {}
};

struct Intersection {
	double t = DBL_MAX;
	Sphere* sphere;
	dvec3 uObj, vObj;
	Intersection(double t, dvec3 u, dvec3 v, Sphere* sphere) :
		t(t), uObj(u), vObj(v), sphere(sphere) {}
};

struct Surface {
	dvec3 hitPoint;
	dvec3 normal;
	Sphere* sphere = nullptr;
	Surface(dvec3 hitPoint, dvec3 normal, Sphere* sphere) : hitPoint(hitPoint), normal(normal), sphere(sphere) {}

	const double DEFAULT_ADJUSTMENT = 0.0001;
	dvec3 adjustedHitPoint(bool outward = true) {
		return hitPoint + normal * (outward ? DEFAULT_ADJUSTMENT : -DEFAULT_ADJUSTMENT);
	}
};

struct Light {
	dvec3 position;
	dvec3 color;
	Light(dvec3 position, dvec3 color) : position(position), color(color) {}
};

struct Refraction {
	dvec3 color;
	double eta = AIR_COEFFICENT;
	Refraction() {}
	Refraction(dvec3 color, double eta) : color(color), eta(eta) {}
};

struct Material {
	// cyan plastic
	dvec3 ambient = dvec3(0, 0.1, 0.06);
	dvec3 diffuse = dvec3(0, 0.50980392, 0.50980392);
	dvec3 specular = dvec3(0.50196078, 0.50196078, 0.50196078);
	double shininess = 32;
	double kr = 0;
	bool procTexture = false;
	Material() {}
	Material(dvec3 diffuse, dvec3 specular, double shininess, double kr)
		: diffuse(diffuse), specular(specular), shininess(shininess), kr(kr) {}
	dvec3 getDiffuse(dvec3 hitPoint) {
		if (procTexture) {
			double pattern = 2 * 3.14 * 5;
			pattern = (cos(hitPoint[1] * pattern) * sin(hitPoint[0] * pattern) + 1) * 0.5;
			return diffuse * pattern;
		}
		return diffuse;
	}
};

struct Group {
	Material material;
	Refraction refraction;
	mat4 xfm = mat4(1);
	Group() {}
	Group(const Group& group) : material(group.material), xfm(mat4(group.xfm)) {}
};

enum class AntiAlias {
	NONE = 0,
	SUPER_SAMPLING = 1,
	ADAPTIVE_SAMPLING = 2
};

//const Material chrome = Material(vec3(0.25f), vec3(0.4f), vec3(0.774597f), 76.8f);
//const Material gold = Material(vec3(0.24725f, 0.1995f, 0.0745f), vec3(0.75164f, 0.60648f, 0.22648f), vec3(0.628281f, 0.555802f, 0.366065f), 12.8f);
//const Material copper = Material(vec3(0.19125f, 0.0735f, 0.0225f), vec3(0.7038f, 0.27048f, 0.0828f), vec3(0.256777f, 0.137622f, 0.086014f), 12.8f);
//const Material jade = Material(vec3(0.135f, 0.2225f, 0.1575f), vec3(0.54f, 0.89f, 0.63f), vec3(0.316228f, 0.316228f, 0.316228f), 12.8f);
//const Material yRubber = Material(vec3(0.05f, 0.05f, 0), vec3(0.5f, 0.5f, 0.4f), vec3(0.7f, 0.7f, 0.04f), 10.0f);