#pragma once

#include "Solid.h"

class MyHouse : public Solid {
private:
	std::vector<unsigned int> mRoof;
	GLuint mIBO2;
protected:
public:
	MyHouse() {
		mIndices = {
			54, 55, 56
		};

		auto quads = triangulate(std::vector<unsigned int> {
			38, 40, 41, 39,
				49, 51, 55, 53,
				48, 52, 54, 50,
		}, 4);

		auto pents = triangulate(std::vector<unsigned int> {
			// walls
			44, 46, 40, 38, 42,
				41, 47, 45, 43, 39,
				// door
				54, 52, 58, 60, 61,
				55, 54, 61, 59, 53,
		}, 5);

		auto hexa = triangulate(std::vector<unsigned int> {
			44, 66, 62, 64, 65, 67,
				67, 63, 68, 70, 71, 72,
				43, 45, 72, 69, 66, 42
		}, 6);

		// walls
		mIndices.insert(mIndices.end(), quads.begin(), quads.end());
		mIndices.insert(mIndices.end(), pents.begin(), pents.end());
		mIndices.insert(mIndices.end(), hexa.begin(), hexa.end());
		glGenBuffers(1, &mIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

		// roof
		mRoof = triangulate(std::vector<unsigned int> {
			47, 41, 40, 46,
				46, 44, 45, 47,
				54, 56, 57, 50,
				51, 57, 56, 55
		}, 4);
		glGenBuffers(1, &mIBO2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mRoof.size() * sizeof(unsigned int), &mRoof[0], GL_STATIC_DRAW);

		mCamera = glm::vec3(30, 10, 30);
		setPointOfInterest();
	}

	~MyHouse() {}

	void render(mat4 vp, bool rotation) {
		setMvp(vp, rotation);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		glUniform3f(sColorLocation, 0, 0.51f, 0.56f);
		glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO2);
		glUniform3f(sColorLocation, 0.78f, 0.16f, 0.16f);
		glDrawElements(GL_TRIANGLES, mRoof.size(), GL_UNSIGNED_INT, (void*)0);
	}
};