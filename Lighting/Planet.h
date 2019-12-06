#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "Solid.h"

class Planet : public Solid {
public:
	Planet(std::vector<unsigned int> indices, int textureId, PlanetSpec spec);

	void static updateRotationSpeed(bool faster) {
		sRotateSpeed *= faster ? 2 : 0.5f;
		sRotateSpeed = clamp(0.05, 50, sRotateSpeed);
	}

	virtual void updateRotationSpeed();

protected:
	static float sRotateSpeed;

	PlanetSpec mSpec;

	float mRotation = 0, mRotationAngle = 0;
	float mRevolution = 0, mRevolutionAngle = 0;
	bool mIsSun = false;

	virtual glm::mat4 getMatrixModel();
	void rotate(mat4& model, float& angle, const float& rate);

private:
	typedef Solid super;

	void computeNormals();

	void setUpData();

	void setUpAttributes(mat4 view, bool rotation);
};