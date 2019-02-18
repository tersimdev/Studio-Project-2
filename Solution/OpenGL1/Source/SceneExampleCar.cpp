#include "SceneExampleCar.h"

SceneExampleCar::SceneExampleCar()
{
}


void SceneExampleCar::InitDerived()
{
	car.Init("OBJ//taxi.obj", "Image//taxi.tga", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1.f, 1.f, 1.f));
	car.CreateRigidBody(Vector3(0, 0, 10), 1200, 0.1f, 0.09f);
	floor.Init("OBJ//ground-low-flat.obj", "Image//color2.tga", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));

	car.SetMaterial(shiny);
	floor.SetMaterial(dull);
}

void SceneExampleCar::RenderDerived()
{
	RenderObject(&floor);
	RenderObject(&car);
	RenderObject(&(car.wheels[0]));

	if (DEBUG)
	{
		std::string temp = "Gear number: " + std::to_string(car.GetGear());
		RenderTextOnScreen(&TEXT, temp, Color(1, 0, 1), 1, 0, 0); //fps
	}
}

void SceneExampleCar::UpdateDerived(double dt)
{
	if (Application::IsKeyPressed(VK_UP) && car.GetGear() > 0)
	{
		car.MoveForward(1, dt);
	}
	else if (Application::IsKeyPressed(VK_UP) && car.GetGear() == 0)
	{
		car.MoveForward(-1, dt);
	}
	else
	{
		car.MoveForward(0, dt);
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		car.Brake(true);
	}
	else
	{
		car.Brake(false);
	}
	
	//todo make actual turn
	if (Application::IsKeyPressed(VK_LEFT))
	{
		car.MoveRight(-1, dt);
	}
	else if (Application::IsKeyPressed(VK_RIGHT))
	{
		car.MoveRight(1, dt);
	}
	else
	{
		car.MoveRight(0, dt);
	}
	car.UpdateSuvat(dt);
	car.UpdateRotation(dt);
	
	if (!currentCam)
		camera[0]->Update(dt, car.GetTranslate(), car.GetAngle()); //update camera
}

void SceneExampleCar::UpdateDerivedBounced(double dt)
{
	if (Application::IsKeyPressed(VK_LCONTROL))
	{
		RequestChangeScene(1);
	}
	if (Application::IsKeyPressed('Z'))
	{
		car.SetGear(car.GetGear() - 1);
	}
	if (Application::IsKeyPressed('X'))
	{
		car.SetGear(car.GetGear() + 1);
	}
}

SceneExampleCar::~SceneExampleCar()
{
}