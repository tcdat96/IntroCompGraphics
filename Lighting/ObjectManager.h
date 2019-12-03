#pragma once

#include <vector>
#include <set>

#include "utils.h"
#include "Planet.h"

constexpr glm::vec3 CAMERA = vec3(70, 25, 70);

class ObjectManager {
private:
	static glm::mat4 sView;

	std::vector<Solid*> mObjects;

	GLuint mVBO;
	GLfloat mLight[18];

	bool mIsSolid = true;
	bool mRotation = false;

	ObjectManager();

	~ObjectManager() {
		for (auto obj : mObjects) {
			delete obj;
		}
	}

	void generateObjects();
	Solid* createPlanet(PlanetSpec info);

public:
	int mVertexCount = 0;

	static ObjectManager* getInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

	void setUniformLocations(GLint model, GLint view, GLint texture, GLint lightSource) {
		Solid::sModelLocation = model;
		Solid::sViewLocation = view;
		Solid::sTextureLocation = texture;
		Solid::sLightSourceLocation = lightSource;
	}

	int readTexture(const char* textureFile);

	void setLightPosition(float x, float y, float z);

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};