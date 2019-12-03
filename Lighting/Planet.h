#pragma once

#include "Solid.h"

class Planet : public Solid {
public:
	Planet(std::vector<unsigned int> indices, int textureId = -1) : Solid(indices) {
		mTextureId = textureId;
	}

private:
	typedef Solid super;

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
};