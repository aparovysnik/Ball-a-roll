#pragma once
#include "GameEntity.h"
class MovingEntity : public GameEntity
{
public:
	MovingEntity(RenderObject* visuals);
	MovingEntity(RenderObject* &visuals, PhysicsObject* &physics);
	virtual ~MovingEntity();

	virtual void Update(float msec);
};

