#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "Solid.h"

#define ROTATE_RATE 0.5f

class Planet : public Solid {
public:
	Planet(std::vector<unsigned int> indices, int textureId, PlanetSpec spec) : Solid(indices) {
		mTextureId = textureId;
		mIsSun = spec.isSun;
		mRotation = spec.rotation > 0 ? ROTATE_RATE / spec.rotation : -1;
		mRevolution = spec.revolution > 0 ? ROTATE_RATE / spec.revolution * 5 : -1;
	}

private:
	typedef Solid super;

	float mRotation = 0, mRotationAngle = 0;
	float mRevolution = 0, mRevolutionAngle = 0;

	bool mIsSun = false;

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

	void setUpAttributes(mat4 view, bool rotation) {
		super::setUpAttributes(view, rotation);
		glUniform1i(Solid::sLightSourceLocation, mIsSun);
	}

	glm::mat4 getMatrixModel() {
		auto model = mat4(1);

		if (mRotation > 0) {
			model = glm::rotate(model, mRevolutionAngle, vec3(0, 1, 0));
			mRotationAngle += mRotation;
			if (mRotationAngle > 3.14 * 2) mRotationAngle = 0;
		}

		model = glm::translate(model, mXfm.translation);
		model = glm::scale(model, mXfm.scale);

		if (mRevolution > 0) {
			mRevolutionAngle += mRevolution;
			if (mRevolutionAngle > 3.14 * 2) mRevolutionAngle = 0;
			model = glm::rotate(model, mRotationAngle, vec3(0, 1, 0));
		}

		return model;
	}
};