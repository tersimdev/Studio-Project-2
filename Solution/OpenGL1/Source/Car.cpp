#include "Car.h"


Car::Car()
{
	for (int i = 0; i < 4; i++)
	{
		wheels[i].Init(MeshBuilder::GenerateCube(Color(0, 1, 0, 0.5f)), "", Vector3(4, 0, 0), Vector3(0, 0, 0), Vector3(0.5, 2, 2));;
		wheels[i].SetRadius(wheelRadius);
	}
}

void Car::RollFront(float u, float v, double dt)
{
	for (int i = 0; i < 2; i++)
	{
		wheels[i].Roll(u, v, dt);
	}
}

void Car::RollBack(float u, float v, double dt)
{
	for (int i = 2; i < 4; i++)
	{
		wheels[i].Roll(u, v, dt);
	}
}

Car::~Car()
{
}