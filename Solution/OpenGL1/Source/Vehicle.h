#ifndef VEHICLE_H
#define VEHICLE_H

#include "RigidBody.h"
#include "Paint.h"

class Vehicle : public RigidBody
{
public:
	Vehicle();
	void MoveForward(int dir, double dt);
	void MoveRight(int dir, double dt);
	void Brake(bool brake);
	
	//getter setters
	void SetStats(float thrustForce, float turningSpeed, float wheelRadius);
	void SetGear(int gear);
	int GetGear();
	int GetThrustForce();
	Vector3 GetAngle(); //ignore x and y
	Paint* GetPaint();

	virtual void RollFront(float u, float v, double dt) = 0;
	virtual void RollBack(float u, float v, double dt) = 0;
	~Vehicle();
protected:
	float thrustForce, turningSpeed;
	int gearNumber;
	float angleY;
	float wheelRadius;
	const float MAX_FORCE = 20000.f;
	const float BRAKE_FRICTION = 10000.f;

	Paint paint;
};

#endif // !VEHICLE_H