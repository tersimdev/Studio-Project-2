#include "CollisionHandler.h"


CollisionHandler* CollisionHandler::instance = nullptr;

CollisionHandler::CollisionHandler()
{
	this->penetrationDist = Vector3(0, 0, 0);
}

CollisionHandler * CollisionHandler::GetInstance()
{
	if (CollisionHandler::instance == nullptr)
	{
		instance = new CollisionHandler;
	}

	return instance;
}

void CollisionHandler::ResolveCollision(RigidBody* A, Collidable* B)
{
	if (simplex.size() == 0)
	{
		std::cout << "Check for collision first!\n";
		return;
	}

	penetrationDist = Vector3(0, 0, 0);
	simplexCopy = simplex;
	this->CalculatePenetration(A, B);

	//A->IncrementTranslate(Vector3(-penetrationDist.x, 0, -penetrationDist.z));
}

//working GJK for 2D and todo EPA
bool CollisionHandler::CheckCollision(RigidBody* A, Collidable* B)
{
	//init var
	direction = INITIAL_DIR;
	pointA = Vector3(0, 0, 0);
	simplex.clear();
	//get first point, and build 0-simplex
	pointA = GetMPoint(A, B, direction);
	if (pointA.Dot(direction) < 0)
		return false; //no intersection
	simplex.emplace_back(pointA);
	direction = -pointA;

	Vector3 b, c, acPerp, abPerp;
	while (true)
	{
		//get next point
		pointA = GetMPoint(A, B, direction);
		//check if the point has passed the origin, if no then no intersect
		if (pointA.Length() == 0)
			return true; //intersect, A and B share same point
		if (pointA.Dot(direction) < 0)
			return false; //no intersection
		//add to simplex
		simplex.emplace_back(pointA);

		//handle line segment, 1-simplex
		if (simplex.size() == 2)
		{
			b = simplex[0];
			direction = TripleCrossProduct(b - pointA, -pointA, b - pointA);
			if (direction.Length() == 0) //if a and b are exactly opposite, cross is zero
				direction = Vector3(b.x - pointA.x, 0, pointA.z - b.z);
			continue; //skip to next iteration
		}

		//handling triangle, 2-simplex
		b = simplex[1];
		c = simplex[0];
		if (b.Length() - c.Length() < 0.0001f && b.Length() - pointA.Length() < 0.0001f) //prevent infinite loop if simplex has all same points
			return true;
		acPerp = TripleCrossProduct(b - pointA, c - pointA, c - pointA);

		if (acPerp.Dot(-pointA) > 0) //if beyond ac
		{
			direction = acPerp;
		}
		else //else beyond ab or in triangle
		{
			abPerp = TripleCrossProduct(c - pointA, b - pointA, b - pointA);

			if (abPerp.Dot(-pointA) < 0) //if within triangle
			{
				return true;
			}
			//else beyond ab
			std::swap(simplex[0], simplex[1]);

			direction = abPerp;
		}

		std::swap(simplex[1], simplex[2]);
		simplex.pop_back(); //remove duplicate in simplex
	}

	return false;
}

bool CollisionHandler::CheckCollision3D(RigidBody* A, Collidable* B)
{
	//init var
	direction = INITIAL_DIR;
	pointA = Vector3(0, 0, 0);
	simplex.clear();
	//get first point, and build 0-simplex
	pointA = GetMPoint(A, B, direction);
	if (pointA.Dot(direction) < 0)
		return false; //no intersection
	simplex.emplace_back(pointA);
	direction = -pointA;

	Vector3 b, c, acPerp, abPerp;
	while (true)
	{
		//get next point
		pointA = GetMPoint(A, B, direction);
		//check if the point has passed the origin, if no then no intersect
		if (pointA.Length() == 0)
			return true; //intersect, A and B share same point
		if (pointA.Dot(direction) < 0)
			return false; //no intersection
		else if (simplex.size() == 4)
			return true; //intersects if the point not past origin, and simplex is alr 3-simplex
		//add to simplex
		simplex.emplace_back(pointA);

		switch (simplex.size())
		{
			//handle line segment, 1-simplex
			case 2:
			{
				b = simplex[0];
				direction = TripleCrossProduct(b - pointA, -pointA, b - pointA);
				if (direction.Length() < TOLERANCE) //if a and b are exactly opposite, cross is zero
					direction = Vector3(b.x - pointA.x, b.y - pointA.y, pointA.z - b.z);
				break;
			}
			//handling triangle, 2-simplex
			case 3:
			{
				b = simplex[1];
				c = simplex[0];
				acPerp = TripleCrossProduct(b - pointA, c - pointA, c - pointA);

				if (acPerp.Dot(-pointA) > 0) //if beyond ac
				{
					direction = acPerp;
				}
				else //else beyond ab or in triangle
				{
					abPerp = TripleCrossProduct(c - pointA, b - pointA, b - pointA);

					if (abPerp.Dot(-pointA) < 0) //if within triangle
					{
						direction = (c - pointA).Cross(b - pointA);
						break;
					}
					direction = abPerp;
					std::swap(simplex[0], simplex[1]);
				}

				std::swap(simplex[1], simplex[2]);
				simplex.pop_back(); //remove duplicate in simplex
				break;
			}
			case 4: //handle 3-simplex
			{
				//I already know origin is within direction of current point if it reaaches this, thus must be in other direction
				direction = -direction;
				break;
			}

		}
	}

	return false;
}

Vector3 CollisionHandler::GetMPoint(Collidable *A, Collidable *B, Vector3 dir)
{
	Vector3 p1 = A->GetCollider()->GetFurthestPoint(dir);
	Vector3 p2 = B->GetCollider()->GetFurthestPoint(-dir);
	return p1 - p2;
}

void CollisionHandler::CalculatePenetration(Collidable *A, Collidable *B)
{
	Vector3 a, b, n;
	double distance;
	//temporarily use abn to find winding (clockwise -1 or anti-clockwise 1)
	a = simplex[0];
	b = simplex[1];
	n = simplex[2];
	
	bool windingCW = ((b - a).Cross(n - a)).y == -1 ? true : false;
	while (true)
	{
		//find closest edge
		Edge closest;
		closest.distance = 99999;
		for (int i = 0; i < simplexCopy.size(); ++i)
		{
			int j = i + 1 == simplexCopy.size() ? 0 : i + 1;
			//get the current point and the next one
			a = simplexCopy[i];
			b = simplexCopy[j];
			n = (windingCW ? Vector3(b.z - a.z, 0, a.x - b.x) : Vector3(a.z - b.z, 0, b.x - a.x));
			// calculate the distance from the origin to the edge
			if (n.Length() == 0)
			{
				std::cout << "Something is wrong with EPA\n";
				return;
			}
			n.Normalize();
			distance = n.Dot(a);
			//check the distance
			if (distance < closest.distance) {
				closest.distance = distance;
				closest.normal = n;
				closest.index = j;
			}
		}
		//support with edge normal
		Vector3 point = GetMPoint(A, B, closest.normal);
		//distance along normal
		distance = point.Dot(closest.normal);
		//if distance close enough to edge distance
		if (distance - closest.distance < TOLERANCE)
		{
			penetrationDist = closest.normal * closest.distance;
			return;
		}
		//else, insert to simplex in between the points that made the closest edge
		else
		{
			simplexCopy.insert(simplexCopy.begin() + closest.index, point);
		}
	}
}

CollisionHandler::~CollisionHandler()
{
}