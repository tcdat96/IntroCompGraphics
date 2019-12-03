#pragma once

#include <vector>
#include <set>

#include "utils.h"
#include "Planet.h"

constexpr float TRANSLATE_DELTA = 6;
constexpr glm::vec3 CAMERA = vec3(60, 25, 60);

class ObjectManager {
private:
	static glm::mat4 sView;

	std::vector<Solid*> mObjects;

	GLuint mVBO;
	GLfloat mLight[18];

	int mViewIndex = 1;
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

	void setViewIndex(unsigned int index);

	void setUniformLocations(GLint model, GLint view, GLint texture) {
		Solid::sModelLocation = model;
		Solid::sViewLocation = view;
		Solid::sTextureLocation = texture;
	}

	int readTexture(const char* textureFile);

	void setLightPosition(float x, float y, float z);

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};