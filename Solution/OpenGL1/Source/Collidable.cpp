#include "Collidable.h"



Collidable::Collidable()
{
	collider = nullptr;
}

void Collidable::DefineBoxCollider(Vector3 size)
{
	if (collider != nullptr) { std::cout << "Already defined a collider!"; return; }
	Vector3 halfSize = size * 0.5f;
	Vector3 points[8] = {
		Vector3(translate.x + halfSize.x, translate.y + halfSize.y, translate.z + halfSize.z),
		Vector3(translate.x + halfSize.x, translate.y + halfSize.y, translate.z - halfSize.z),
		Vector3(translate.x - halfSize.x, translate.y + halfSize.y, translate.z - halfSize.z),
		Vector3(translate.x - halfSize.x, translate.y + halfSize.y, translate.z + halfSize.z),

		Vector3(translate.x + halfSize.x, translate.y - halfSize.y, translate.z + halfSize.z),
		Vector3(translate.x + halfSize.x, translate.y - halfSize.y, translate.z - halfSize.z),
		Vector3(translate.x - halfSize.x, translate.y - halfSize.y, translate.z - halfSize.z),
		Vector3(translate.x - halfSize.x, translate.y - halfSize.y, translate.z + halfSize.z),
	};
	
	collider = new Collider;

	this->collider->SetScale(halfSize);
	for (int i = 0; i < 8; ++i)
		this->collider->PushToHullPoints(points[i]);
	this->AddChild(collider);
}

void Collidable::DefineSphereCollider(Vector3 radius)
{
	if (collider != nullptr) { std::cout << "Already defined a collider!"; return; }

	Vector3 points[6] = {
		Vector3(translate.x + radius.x, 0, 0),
		Vector3(translate.x - radius.x, 0, 0),
		Vector3(0, translate.y + radius.y, 0),
		Vector3(0, translate.y - radius.y, 0),
		Vector3(0, 0, translate.z + radius.z),
		Vector3(0, 0, translate.z - radius.z),
	};

	collider = new Collider;

	this->collider->SetScale(radius);
	for (int i = 0; i < 6; ++i)
		this->collider->PushToHullPoints(points[i]);
	this->AddChild(collider);
}


void Collidable::UpdateCollider()
{
	collider->UpdateHull(this->translate, this->rotate);
}

Collidable::~Collidable()
{
}
