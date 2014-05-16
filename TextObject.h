#pragma once
#include "PhysicsObject.h"
class TextObject : public PhysicsObject
{
public:
	TextObject(Vector3 position, BoundingBox bBox) : PhysicsObject(position, bBox)
	{
	}

	~TextObject()
	{
	}

	virtual std::unique_ptr<PhysicsObject> Clone()
	{
		TextObject* clone = new TextObject(GetPosition(), boundingBox);
		clone->SetAcceleration(GetAcceleration());
		if (IsMoving())
			clone->ActivateMovement();
		clone->SetWeight(GetWeight());
		clone->SetSpeed(GetSpeed());

		clone->SetVerletSpeed(GetVerletSpeed());

		return std::unique_ptr<TextObject>(clone);
	}
};

