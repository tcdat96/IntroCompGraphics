#pragma once

#include <vector>
#include <set>

#include "utils.h"
#include "MyHouse.h"

constexpr float TRANSLATE_DELTA = 6;


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

public:
	int mVertexCount = 0;

	static ObjectManager* getInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

	void setViewIndex(unsigned int index);

	void setModelViewLocation(GLint model, GLint view) {
		Solid::sModelLocation = model;
		Solid::sViewLocation = view;
	}

	void setColorLocation(GLint location) {
		Solid::sColorLocation = location;
	}

	void setLightPosition(float x, float y, float z);

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};