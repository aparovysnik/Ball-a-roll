#include "MovingEntity.h"
#include "PhysicsContainer.h"


MovingEntity::MovingEntity(RenderObject* visuals) : GameEntity(visuals)
{

}

MovingEntity::MovingEntity(RenderObject* &visuals, PhysicsObject* &physics) : GameEntity(visuals, physics)
{
}


MovingEntity::~MovingEntity()
{
}

void MovingEntity::Update(float msec)
{
	PhysicsContainer *physicsWorld = &PhysicsContainer::Instance();
	physicsWorld->VerletDisplacement(physics, msec);
	GameEntity::Update(msec);
}

