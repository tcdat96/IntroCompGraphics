#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <glew/glew.h>

class Solid {
protected:
	GLuint mIBO = 0;
	std::vector<unsigned int> mIndices;

	GLuint mMvpLocation = 0;
	glm::vec3 mCamera = glm::vec3(5, 2, 5);

	float mAngle = 0;
	float mTranslateX = 0;

	glm::mat4 mVP = glm::mat4(1);
	glm::mat4 mView = glm::mat4(1);
	glm::mat4 mModel = glm::mat4(1);

	void setPointOfInterest(vec3 pointOfInterest = vec3(0));

	void setMvp(mat4 vp, bool rotation);
	std::vector<unsigned int> triangulate(std::vector<unsigned int> indices, int vpf);

public:
	Solid() {}
	Solid(std::vector<unsigned int> indices, int vpf = 3);
	~Solid();

	std::vector<unsigned int> getIndices() {
		return mIndices;
	}

	void setMvpLocation(GLuint location) {
		mMvpLocation = location;
	}
	void setProjectionMatrix(glm::mat4 projection) {
		mVP = projection * mView;
	}

	void setInitialTransX(float x);
	void render(bool rotation);
	void render(mat4 vp);
	virtual void render(mat4 vp, bool rotation);
};