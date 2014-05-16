#include "PlainObject.h"


PlainObject::PlainObject(btRigidBody* body, Vector3 position, vector<Vector3> edgePoints, BoundingBox bBox, Vector3 normal, Game::Material material) : PhysicsObject(body, position, bBox, material)
{
	type = Game::Type::Still;
	this->normal = normal;
	this->edgePoints = edgePoints;
}


PlainObject::~PlainObject()
{
}

Game::CollisionSettings* PlainObject::CollidesWith(PhysicsObject* obj)
{
	return NULL;
}