#pragma once

#include "Planet.h"

Planet::Planet(std::vector<unsigned int> indices, int textureId, PlanetSpec spec) : Solid(indices) {
	mTextureId = textureId;
	mIsSun = spec.isSun;
	mRotation = spec.rotation > 0 ? ROTATE_RATE / spec.rotation / 10 : -1;
	mRevolution = spec.revolution > 0 ? ROTATE_RATE / spec.revolution : -1;
}

glm::mat4 Planet::getMatrixModel() {
	auto model = mat4(1);

	rotate(model, mRevolutionAngle, mRevolution);

	model = glm::translate(model, mXfm.translation);
	model = glm::scale(model, mXfm.scale);

	rotate(model, mRotationAngle, mRotation);

	return model;
}

void Planet::rotate(mat4& model, float& angle, const float& rate) {
	if (rate > 0) {
		model = glm::rotate(model, angle, vec3(0, 1, 0));
		angle += rate;
		if (angle > 3.14 * 2) angle = 0;
	}
}

void Planet::computeNormals() {
	// do nothing
}

void Planet::setUpData() {
	int vSize = sizeof(GLfloat) * 3;
	for (unsigned int i = 0; i < mIndices.size(); i++) {
		auto data = &vPositions[mIndices[i] * 3];
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2, vSize, data);
		glBufferSubData(GL_ARRAY_BUFFER, vSize * i * 2 + vSize, vSize, data);
	}
}

void Planet::setUpAttributes(mat4 view, bool rotation) {
	super::setUpAttributes(view, rotation);
	glUniform1i(Solid::sLightSourceLocation, mIsSun);
}