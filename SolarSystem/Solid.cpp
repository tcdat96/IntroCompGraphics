#include "Solid.h"

GLint Solid::sModelLocation;
GLint Solid::sViewLocation;
GLint Solid::sTextureLocation;
GLint Solid::sLightSourceLocation;

std::vector<GLfloat> Solid::vPositions;
unsigned int Solid::sVertexCount = 0;

Solid::~Solid()
{
	
}

Solid::Solid(std::vector<unsigned int> indices, int vpf)
{
	mIndices = vpf == 3 ? indices : triangulate(indices, vpf);
	sVertexCount += mIndices.size();

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

void Solid::computeNormals() {
	auto normals = computeNormals(mIndices);
	vNormals.insert(vNormals.end(), normals.begin(), normals.end());
}

std::vector<glm::vec3> Solid::computeNormals(std::vector<unsigned int> indices)
{
	std::vector<glm::vec3> normals;
	glm::vec3 v[3];
	for (unsigned int i = 0; i < indices.size(); i += 3) {
		for (int j = 0; j < 3; j++) {
			int index = indices[i + j] * 3;
			v[j] = { vPositions[index], vPositions[index + 1], vPositions[index + 2] };
		}
		normals.push_back(glm::cross(v[1] - v[0], v[2] - v[0]));
	}
	return normals;
}

void Solid::setPointOfInterest(vec3 pointOfInterest)
{
	pointOfInterest += mXfm.translation;
	mCamera += mXfm.translation;
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	mView = glm::lookAt(mCamera, pointOfInterest, cameraUp);
}

void Solid::transform(Transformation xfm) {
	mXfm = xfm;
	setPointOfInterest();
}

void Solid::setUpData() {
	int vSize = sizeof(GLfloat) * 3;
	for (unsigned int i = 0; i < mIndices.size(); i++) {
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2, vSize, &vPositions[mIndices[i] * 3]);
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2 + vSize, vSize, &vNormals[i / 3]);
	}
}

void Solid::setUpAttributes(mat4 view, bool rotation)
{
	if (rotation) {
		if (mAngle > M_PI * 2) mAngle = 0;
		else mAngle += 0.001f;
	}

	auto model = getMatrixModel();
	glUniformMatrix4fv(sModelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(sViewLocation, 1, GL_FALSE, glm::value_ptr(view));

	if (mTextureId >= 0) {
		glUniform1i(sTextureLocation, mTextureId);
	}
}

glm::mat4 Solid::getMatrixModel() {
	auto model = glm::scale(mat4(1), mXfm.scale);
	model = glm::translate(model, mXfm.translation);
	if (mAngle > 0) {
		model = glm::rotate(model, mAngle, vec3(0, 1, 0));
	}
	return model;
}

void Solid::render(bool rotation)
{
	render(mView, rotation);
}

void Solid::render(mat4 view, bool rotation)
{
	setUpData();
	setUpAttributes(view, rotation);
	glDrawArrays(GL_TRIANGLES, 0, mIndices.size());
}