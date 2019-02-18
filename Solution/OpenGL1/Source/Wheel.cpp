#include "Wheel.h"


Wheel::Wheel()
{
	omegaI = omegaF = theta = 0;
	radius = 0.5f;
}

void Wheel::Roll(float u, float v, double dt)
{
	omegaF = v / radius;
	omegaI = u / radius;

	theta = 0.5 * (omegaI + omegaF) * dt;

	this->rotate = Vector3(theta, 0, 0);
}

void Wheel::SetRadius(float radius)
{
	this->radius = radius;
}

Wheel::~Wheel()
{
}