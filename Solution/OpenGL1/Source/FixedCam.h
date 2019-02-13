#ifndef FIXED_CAM_H
#define FIXED_CAM_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "Camera.h"

class FixedCam : public Camera
{
public:

	FixedCam();
	~FixedCam();
	virtual void Update(double dt, Vector3 translate, Vector3 rotate);
	virtual void Reset();

private:
	virtual void InitDerived();
	
	Vector3 offset = Vector3(0, 10, -10);

	Mtx44 rotationMatrixP;
};

#endif
