#include "ObjectManager.h"

glm::mat4 ObjectManager::sView = glm::lookAt(
	CAMERA,
	vec3(TRANSLATE_DELTA * 4, 0, 0),
	vec3(0, 1, 0)
);

ObjectManager::ObjectManager() {
	float phi = (1 + (float)sqrt(5)) / 2;
	float iphi = 1.0f / phi;
	Solid::vPositions = {};

	generateObjects();

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 10000 * 2 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
}

void ObjectManager::generateObjects()
{
	Solid* sphere = generateSphere();

	mObjects.push_back(sphere);
}

Solid* ObjectManager::generateSphere() {
	int stacks = 10;
	int slices = stacks;
	float radius = 6;

	int offset = (Solid::vPositions.size()) / 3;

	for (int i = 0; i <= stacks; ++i) {

		float V = i / (float)stacks;
		float phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			float U = j / (float)slices;
			float theta = U * (glm::pi <float>() * 2);

			float x = cosf(theta) * sinf(phi);
			float y = cosf(phi);
			float z = sinf(theta) * sinf(phi);

			Solid::vPositions.push_back(x * radius);
			Solid::vPositions.push_back(y * radius);
			Solid::vPositions.push_back(z * radius);
		}
	}

	std::vector<unsigned int> indices;
	for (int i = 0; i < slices * stacks + slices; ++i) {
		indices.push_back(i);
		indices.push_back(i + slices + 1);
		indices.push_back(i + slices);

		indices.push_back(i + slices + 1);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	for (unsigned int i = 0; i < indices.size(); i++) {
		indices[i] += offset;
	}
	
	return new Sphere(indices);
}

void ObjectManager::setLightPosition(float x, float y, float z) {
	mLight[0] = x; mLight[1] = y; mLight[2] = z;
	mLight[6] = x + 1; mLight[7] = y; mLight[8] = z - 1;
	mLight[12] = x - 1; mLight[13] = y; mLight[14] = z - 1;

	glm::vec3 v0 = vec3(mLight[0], mLight[1], mLight[2]);
	glm::vec3 v1 = vec3(mLight[6], mLight[7], mLight[8]);
	glm::vec3 v2 = vec3(mLight[12], mLight[13], mLight[14]);
	glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

	for (int i = 0; i < 3; i++) {
		mLight[3 + i * 6] = normal[0];
		mLight[4 + i * 6] = normal[1];
		mLight[5 + i * 6] = normal[2];
	}
}

void ObjectManager::setViewIndex(unsigned int index) {
	if (index > 0 && index <= mObjects.size() + 1 && index != mViewIndex) {
		mViewIndex = index;
	}
}

void ObjectManager::render() {
	if (mViewIndex > 1) {
		mObjects[mViewIndex - 2]->render(mRotation);
	}
	else {
		// debugging
		glUniformMatrix4fv(Solid::sViewLocation, 1, GL_FALSE, glm::value_ptr(sView));
		glUniformMatrix4fv(Solid::sModelLocation, 1, GL_FALSE, glm::value_ptr(mat4(1)));
		glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof(GLfloat), mLight);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
			(*it)->render(sView, mRotation);
		}
	}
}
