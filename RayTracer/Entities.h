#pragma once

#include "Sphere.h"

class Sphere;

struct Pixel {
	int r, g, b;
};

struct Ray {
	vec3 u;
	vec3 v;
	Sphere* sphere = nullptr;
	float t = FLT_MAX;
	Ray(vec3 u) : u(u) {}
};

struct Material {
	// cyan plastic
	vec3 ambient = vec3(0, 0.1, 0.06);
	vec3 diffuse = vec3(0, 0.50980392, 0.50980392);
	vec3 specular = vec3(0.50196078, 0.50196078, 0.50196078);
	float shininess = 32;
	Material() {}
	Material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess)
		: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
};

const Material chrome = Material(vec3(0.25f), vec3(0.4f), vec3(0.774597f), 76.8f);
const Material gold = Material(vec3(0.24725f, 0.1995f, 0.0745f), vec3(0.75164f, 0.60648f, 0.22648f), vec3(0.628281f, 0.555802f, 0.366065f), 12.8f);
const Material copper = Material(vec3(0.19125f, 0.0735f, 0.0225f), vec3(0.7038f, 0.27048f, 0.0828f), vec3(0.256777f, 0.137622f, 0.086014f), 12.8f);
const Material jade = Material(vec3(0.135f, 0.2225f, 0.1575f), vec3(0.54f, 0.89f, 0.63f), vec3(0.316228f, 0.316228f, 0.316228f), 12.8f);
const Material yRubber = Material(vec3(0.05f, 0.05f, 0), vec3(0.5f, 0.5f, 0.4f), vec3(0.7f, 0.7f, 0.04f), 10.0f);