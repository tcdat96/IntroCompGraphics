#pragma once

#include <string>

#ifdef DEBUG
#  define DEBUG_PRINT(x) cerr << x
#else
#  define DEBUG_PRINT(x)
#endif

#define SIMILAR_THRESHOLD 0.1

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

using namespace std;

bool startsWith(string src, string prefix) {
	return strncmp(src.c_str(), prefix.c_str(), prefix.size()) == 0;
}

bool equals(double a, double b) {
	return abs(a - b) < 0.0000001;
}

bool similar(dvec3 color1, dvec3 color2) {
	dvec3 diff = color1 - color2;
	return abs(diff[0]) < SIMILAR_THRESHOLD && abs(diff[1]) < SIMILAR_THRESHOLD && abs(diff[1]) < SIMILAR_THRESHOLD;
}

bool similar(dvec3 color1, dvec3 color2, dvec3 color3, dvec3 color4) {
	return similar(color1, color2) && similar(color1, color3) && similar(color1, color4)
		&& similar(color2, color3) && similar(color2, color4) && similar(color3, color4);
}