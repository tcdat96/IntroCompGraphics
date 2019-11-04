#include "Solid.h"

GLint Solid::sModelLocation;
GLint Solid::sViewLocation;
GLint Solid::sAmbientLocation;
GLint Solid::sDiffuseLocation;
GLint Solid::sSpecularLocation;
GLint Solid::sShininessLocation;
std::vector<GLfloat> Solid::vPositions;

Solid::~Solid()
{
	
}

Solid::Solid(std::vector<unsigned int> indices, int vpf)
{
	mIndices = vpf == 3 ? indices : triangulate(indices, vpf);

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
	pointOfInterest += vec3(mTranslateX, 0, 0);
	mCamera += vec3(mTranslateX, 0, 0);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	mView = glm::lookAt(mCamera, pointOfInterest, cameraUp);
}

void Solid::prepareDraw(mat4 view, bool rotation)
{
	if (rotation) {
		if (mAngle > M_PI * 2) mAngle = 0;
		else mAngle += 0.001f;
	}

	auto model = mTranslateX == 0 ? mat4(1) : glm::translate(mat4(1), vec3(mTranslateX, 0, 0));
	if (mAngle > 0) {
		model = glm::rotate(model, mAngle, vec3(0, 1, 0));
	}

	glUniformMatrix4fv(sModelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(sViewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::vec3 ambient = mMaterial.ambient;
	glUniform3f(sAmbientLocation, ambient[0], ambient[1], ambient[2]);
	glm::vec3 diffuse = mMaterial.diffuse;
	glUniform3f(sDiffuseLocation, diffuse[0], diffuse[1], diffuse[2]);
	glm::vec3 specular = mMaterial.specular;
	glUniform3f(sSpecularLocation, specular[0], specular[1], specular[2]);

	glUniform1f(sShininessLocation, mMaterial.shininess);
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
	int vSize = sizeof(GLfloat) * 3;
	for (unsigned int i = 0; i < mIndices.size(); i++) {
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2, vSize, &vPositions[mIndices[i] * 3]);
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2 + vSize, vSize, &vNormals[i / 3]);
	}

	prepareDraw(view, rotation);

	glDrawArrays(GL_TRIANGLES, 0, mIndices.size());
}