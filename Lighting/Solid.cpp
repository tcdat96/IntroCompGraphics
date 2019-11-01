#include "Solid.h"

GLint Solid::sModelViewLocation;
GLint Solid::sColorLocation;
std::vector<GLfloat> Solid::vPositions;

Solid::~Solid()
{
	
}

Solid::Solid(std::vector<unsigned int> indices, int vpf)
{
	mIndices = vpf == 3 ? indices : triangulate(indices, vpf);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mIndices.size() * 3 * sizeof(GLfloat) * 2, NULL, GL_STATIC_DRAW);

	mColor[0] = rand(0.25, 1);
	mColor[1] = rand(0.25, 1);
	mColor[2] = rand(0.25, 1);

	computeNormals();

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

void Solid::computeNormals()
{
	for (unsigned int i = 0; i < mIndices.size();) {
		glm::vec3 v[3];
		for (int j = 0; j < 3; j++) {
			int index = mIndices[i++] * 3;
			v[j] = { vPositions[index], vPositions[index + 1],vPositions[index + 2] };
		}
		vNormals.push_back(glm::cross(v[1] - v[0], v[2] - v[1]));
	}
}

void Solid::setPointOfInterest(vec3 pointOfInterest)
{
	pointOfInterest += vec3(mTranslateX, 0, 0);
	mCamera += vec3(mTranslateX, 0, 0);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	mView = glm::lookAt(mCamera, pointOfInterest, cameraUp);
}

void Solid::setMvp(mat4 view, bool rotation)
{
	if (rotation) {
		if (mAngle > M_PI * 2) mAngle = 0;
		else mAngle += 0.001f;
	}

	auto model = mTranslateX == 0 ? mat4(1) : glm::translate(mat4(1), vec3(mTranslateX, 0, 0));
	if (mAngle > 0) {
		model = glm::rotate(model, mAngle, vec3(0, 1, 0));
	}

	auto modelView = view * model;
	glUniformMatrix4fv(sModelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView));

	glUniform3f(sColorLocation, mColor[0], mColor[1], mColor[2]);
}

void Solid::setInitialTransX(float x) {
	mTranslateX = x;
	setPointOfInterest();
}

void Solid::render(mat4 view)
{
	render(view, false);
}

void Solid::render(bool rotation)
{
	render(mView, rotation);
}

void Solid::render(mat4 view, bool rotation)
{
	glBindBuffer(GL_VERTEX_ARRAY, mVBO);
	int vSize = sizeof(GLfloat) * 3;
	for (unsigned int i = 0; i < mIndices.size(); i++) {
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2, vSize, &vPositions[mIndices[i] * 3]);
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2 + vSize, vSize, &vNormals[i / 3]);
	}

	setMvp(view, rotation);

	glDrawArrays(GL_TRIANGLES, 0, mIndices.size());
}