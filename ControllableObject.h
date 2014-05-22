#pragma once
#include "PhysicsObject.h"
#include "Controller.h"

class ControllableObject : public PhysicsObject
{
public:
	ControllableObject(btRigidBody* body, Vector3 position, BoundingBox bBox, Game::Material material);
	~ControllableObject();

	virtual std::unique_ptr<PhysicsObject> Clone()
	{
		ControllableObject* clone = new ControllableObject(body, GetPosition(), boundingBox, material);
		clone->SetAcceleration(GetAcceleration());
		if (IsMoving())
			clone->ActivateMovement();
		clone->SetWeight(GetWeight());
		clone->SetSpeed(GetSpeed());
		clone->SetVerletSpeed(GetVerletSpeed());

		return std::unique_ptr<ControllableObject>(clone);
	}
	
	virtual void Update(float msec, set<Action>actions);

	virtual Game::CollisionSettings* CollidesWith(PhysicsObject* obj) { return NULL; }

	Vector3 GetMovement()
	{
		btVector3 v = body->getAngularVelocity();
		btVector3 v1 = body->getLinearVelocity();
		return positions.at(positions.size() - 1) - positions.at(positions.size() - 2);
	}

private:
	Vector3 fwDir;
	Vector3 bwDir;
	Vector3 lfDir;
	Vector3 rtDir;
	Vector3 upDir;
};

