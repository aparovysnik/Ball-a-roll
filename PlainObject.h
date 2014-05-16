#pragma once
#include "PhysicsObject.h"
class PlainObject : public PhysicsObject
{
public:
	PlainObject(btRigidBody* body, Vector3 position, vector<Vector3> edgePoints, BoundingBox bBox, Vector3 normal, Game::Material material);
	~PlainObject();

	virtual Game::CollisionSettings* CollidesWith(PhysicsObject* obj);

	Vector3 GetNormal() { return normal; }
	vector<Vector3> GetEdgePoints() { return edgePoints; }

	virtual std::unique_ptr<PhysicsObject> Clone()
	{
		PlainObject* clone = new PlainObject(body, GetPosition(), GetEdgePoints(), boundingBox, GetNormal(), material);
		clone->SetAcceleration(GetAcceleration());
		if (IsMoving())
			clone->ActivateMovement();
		clone->SetWeight(GetWeight());
		clone->SetSpeed(GetSpeed());
		clone->SetVerletSpeed(GetVerletSpeed());
		return std::unique_ptr<PlainObject>(clone);
	}

private:
	Vector3 normal;
	vector<Vector3> edgePoints;
};

