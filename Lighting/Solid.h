#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include "utils.h"
#include "Material.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include <glew/glew.h>

class Solid {
protected:
	std::vector<unsigned int> mIndices;
	std::vector<glm::vec3> vNormals;
	GLfloat mColor[3] = { 1, 0, 0 };

	glm::vec3 mCamera = glm::vec3(5, 2, 5);

	float mAngle = 0;
	float mTranslateX = 0;

	Material mMaterial;

	glm::mat4 mView = glm::mat4(1);

	void computeNormals();
	static std::vector<glm::vec3> computeNormals(std::vector<unsigned int> indices);

	void setPointOfInterest(vec3 pointOfInterest = vec3(0));

	void prepareDraw(mat4 vp, bool rotation);
	std::vector<unsigned int> triangulate(std::vector<unsigned int> indices, int vpf);

public:
	static GLuint sVBO;
	static std::vector<GLfloat> vPositions;
	static GLint sViewLocation;
	static GLint sModelLocation;
	static GLint sAmbientLocation;
	static GLint sDiffuseLocation;
	static GLint sSpecularLocation;
	static GLint sShininessLocation;

	Solid() {}
	Solid(std::vector<unsigned int> indices, int vpf = 3);
	~Solid();

	std::vector<unsigned int> getIndices() {
		return mIndices;
	}

	void setInitialTransX(float x);

	void setMaterial(Material material) {
		mMaterial = material;
	}

	void render(bool rotation);
	void render(mat4 vp);
	virtual void render(mat4 vp, bool rotation);
};