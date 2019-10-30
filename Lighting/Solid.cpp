#include "Solid.h"

GLint Solid::sMvpLocation = 1;
GLint Solid::sColorLocation = 2;

Solid::~Solid()
{
	
}

Solid::Solid(std::vector<unsigned int> indices, int vpf)
{
	mIndices = vpf == 3 ? indices : triangulate(indices, vpf);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	mColor[0] = rand(0, 1);
	mColor[1] = rand(0, 1);
	mColor[2] = rand(0, 1);

	setPointOfInterest();
}

std::vector<unsigned int> Solid::triangulate(std::vector<unsigned int> indices, int vpf)
{
	std::vector<unsigned int> result;
	for (unsigned int i = 0; i < indices.size(); i += vpf) {
		for (int j = 1; j < vpf - 1; j++) {
			result.push_back(indices[i]);
			result.push_back(indices[i + j]);
			result.push_back(indices[i + j + 1]);
		}
	}
	return result;
}

void Solid::setPointOfInterest(vec3 pointOfInterest)
{
	pointOfInterest += vec3(mTranslateX, 0, 0);
	mCamera += vec3(mTranslateX, 0, 0);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	mView = glm::lookAt(mCamera, pointOfInterest, cameraUp);
}

void Solid::setMvp(mat4 vp, bool rotation)
{
	if (rotation) {
		if (mAngle > M_PI * 2) mAngle = 0;
		else mAngle += 0.001f;
	}

	mModel = mTranslateX == 0 ? mat4(1) : glm::translate(mat4(1), vec3(mTranslateX, 0, 0));
	if (mAngle > 0) {
		mModel = glm::rotate(mModel, mAngle, vec3(0, 1, 0));
	}

	auto mvp = vp * mModel;
	glUniformMatrix4fv(sMvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform3f(sColorLocation, mColor[0], mColor[1], mColor[2]);
}

void Solid::setInitialTransX(float x) {
	mTranslateX = x;
	setPointOfInterest();
}

void Solid::render(mat4 vp)
{
	render(vp, false);
}

void Solid::render(bool rotation)
{
	render(mVP, rotation);
}

void Solid::render(mat4 vp, bool rotation)
{
	setMvp(vp, rotation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, (void*)0);
}