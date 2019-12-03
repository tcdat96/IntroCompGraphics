#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include "utils.h"

#include <glew/glew.h>

class Solid {
protected:
	std::vector<unsigned int> mIndices;
	std::vector<glm::vec3> vNormals;
	GLfloat mColor[3] = { 1, 0, 0 };

	glm::vec3 mCamera = glm::vec3(5, 2, 5);

	float mAngle = 0;
	Transformation mXfm;

	glm::mat4 mView = glm::mat4(1);

	int mTextureId = -1;

	std::vector<unsigned int> triangulate(std::vector<unsigned int> indices, int vpf);

	virtual void computeNormals();
	static std::vector<glm::vec3> computeNormals(std::vector<unsigned int> indices);

	void setPointOfInterest(vec3 pointOfInterest = vec3(0));

	virtual void setUpData();
	virtual void setUpAttributes(mat4 vp, bool rotation);
	virtual glm::mat4 getMatrixModel();

public:
	static GLuint sVBO;
	static std::vector<GLfloat> vPositions;
	static unsigned int sVertexCount;
	static GLint sViewLocation;
	static GLint sModelLocation;
	static GLint sTextureLocation;

	Solid() {}
	Solid(std::vector<unsigned int> indices, int vpf = 3);
	~Solid();

	std::vector<unsigned int> getIndices() {
		return mIndices;
	}

	void setTextureId(int textureId) {
		mTextureId = textureId;
	}

	void transform(Transformation xfm);

	virtual void render(bool rotation);
	virtual void render(mat4 vp, bool rotation);
};