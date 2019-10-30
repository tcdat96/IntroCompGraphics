#pragma once

#include <vector>
#include <set>

#include "utils.h"
#include "MyHouse.h"

constexpr float TRANSLATE_DELTA = 6;


class ObjectManager {
private:
	static glm::mat4 sVP;

	std::vector<GLfloat> vPositions;
	std::vector<Solid*> mObjects;

	GLuint VBO;
	GLuint IBO;

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

	int getBufferSize();

public:
	int mVertexCount = 0;

	static ObjectManager* getInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

	GLuint getVbo() {
		return VBO;
	}

	int getColorPtr() {
		return getBufferSize();
	}

	void setViewIndex(unsigned int index);

	void setProjectionMatrix(glm::mat4 projection);

	void setColorLocation(GLuint location) {
		Solid::sColorLocation = location;
	}

	void setMvpLocation(GLuint location) {
		Solid::sMvpLocation = location;
	}

	void toggleRotation() {
		mRotation = !mRotation;
	}

	void render();
};