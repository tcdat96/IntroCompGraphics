#pragma once

#include "Planet.h"

class Satellite : public Planet {
private:
	typedef Planet super;

	float mCenterRev = 0;
	float mCenterRevAngle = 0;
	float mCenterRevRate;
	float mDistance;
public:
	Satellite(std::vector<unsigned int> indices, int textureId, PlanetSpec satSpec, PlanetSpec center) :
		Planet(indices, textureId, satSpec) {
		mCenterRev = center.revolution;
		mCenterRevRate = sRotateSpeed / center.revolution;
		mDistance = center.xfm.scale.x + 1;
	}

	virtual void updateRotationSpeed() {
		super::updateRotationSpeed();
		mCenterRevRate = sRotateSpeed / mCenterRev;
	}

	virtual glm::mat4 getMatrixModel() override {
		auto model = mat4(1);

		rotate(model, mCenterRevAngle, mCenterRevRate);

		model = glm::translate(model, mXfm.translation);
		rotate(model, mRevolutionAngle, mRevolution);
		model = glm::translate(model, vec3(0, 0, -mXfm.translation.z + 3));

		model = glm::translate(model, mXfm.translation);
		model = glm::scale(model, mXfm.scale);

		rotate(model, mRotationAngle, mRotation);

		return model;
	}
};