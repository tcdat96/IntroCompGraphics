#pragma once

#include <iostream>
#include <fstream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

struct Transformation {
	vec3 translation = vec3(0);
	vec3 scale = vec3(1);
	Transformation() {}
	Transformation(vec3 translation, vec3 scale) : translation(translation), scale(scale) {}
};

struct PlanetSpec {
	const char* texture;
	Transformation xfm;
	PlanetSpec(const char* texture, float scale, float z) : texture(texture) {
		xfm = Transformation(vec3(0, 0, z), vec3(scale));
	}
};

float rand(float start, float end);
char* readFile(const char* fileName);
bool inTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);