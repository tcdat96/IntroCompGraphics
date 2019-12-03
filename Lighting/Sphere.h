#pragma once

#include "Solid.h"

class Sphere : public Solid {
public:
	Sphere(std::vector<unsigned int> indices, int textureId = -1) : Solid(indices) {
		mTextureId = textureId;
	}

private:
	typedef Solid super;

	bool mScale = false;

	void computeNormals() {
		// do nothing
	}

	void setUpData() {
		int vSize = sizeof(GLfloat) * 3;
		for (unsigned int i = 0; i < mIndices.size(); i++) {
			auto data = &vPositions[mIndices[i] * 3];
			glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2, vSize, data);
			glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2 + vSize, vSize, data);
		}
	}

	glm::mat4 getMatrixModel() {
		auto model = super::getMatrixModel();
		if (mScale) {
			float scale = 1.0f / 4;
			model = glm::scale(model, vec3(scale, scale, scale));
			mScale = false;
		}
		return model;
	}

	virtual void setUpAttributes(mat4 vp, bool rotation) {
		mScale = true;
		super::setUpAttributes(vp, rotation);
	}
};