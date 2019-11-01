#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include "utils.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <glew/glew.h>

class Solid {
protected:
	GLuint mVBO = 0;
	std::vector<unsigned int> mIndices;
	std::vector<glm::vec3> vNormals;
	GLfloat mColor[3] = { 1, 0, 0 };

	glm::vec3 mCamera = glm::vec3(5, 2, 5);

	float mAngle = 0;
	float mTranslateX = 0;

	glm::mat4 mView = glm::mat4(1);

	void computeNormals();

	void setPointOfInterest(vec3 pointOfInterest = vec3(0));

	void setMvp(mat4 vp, bool rotation);
	std::vector<unsigned int> triangulate(std::vector<unsigned int> indices, int vpf);

public:
	static std::vector<GLfloat> vPositions;
	static GLint sModelViewLocation;
	static GLint sColorLocation;

	Solid() {}
	Solid(std::vector<unsigned int> indices, int vpf = 3);
	~Solid();

	std::vector<unsigned int> getIndices() {
		return mIndices;
	}

	void setInitialTransX(float x);
	void render(bool rotation);
	void render(mat4 vp);
	virtual void render(mat4 vp, bool rotation);
};