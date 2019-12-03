#include "ObjectManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

glm::mat4 ObjectManager::sView = glm::lookAt(CAMERA, vec3(0), vec3(0, 1, 0));

ObjectManager::ObjectManager() {
	float phi = (1 + (float)sqrt(5)) / 2;
	float iphi = 1.0f / phi;
	Solid::vPositions = {};

	generateObjects();

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 100000 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
}

void ObjectManager::generateObjects()
{
	const char* textures[] = {
		"textures/sun.jpg",
		"textures/mercury.jpg",
		"textures/venus.jpg",
		"textures/earth.jpg",
		"textures/mars.jpg",
		"textures/jupiter.jpg",
		"textures/saturn.jpg",
		"textures/uranus.jpg",
		"textures/neptune.jpg"
	};
	for (auto texture : textures) {
		Solid* solid = generateSphere(texture);
		mObjects.push_back(solid);
	}

	for (int i = 1; i < mObjects.size(); i++) {
		auto translation = vec3(0, 0, TRANSLATE_DELTA * i);
		std::cout << glm::to_string(translation) << std::endl;
		mObjects[i]->translate(translation);
	}
}

Solid* ObjectManager::generateSphere(const char* texture) {
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
	
	int textureId = readTexture(texture);
	return new Sphere(indices, textureId);
}

int ObjectManager::readTexture(const char* textureFile) {
	int w;
	int h;
	int comp;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* image = stbi_load(textureFile, &w, &h, &comp, STBI_rgb_alpha);
	if (image == nullptr) {
		throw(std::string("Failed to load texture"));
	}
	
	int textureId = mObjects.size();
	glActiveTexture(GL_TEXTURE0 + textureId);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);

	return textureId++;
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

		for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
			(*it)->render(sView, mRotation);
		}
	}
}
