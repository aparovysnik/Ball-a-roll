#include "PhysicsObject.h"
#include "ControllableObject.h"
#include "PlainObject.h"
#include "PhysicsContainer.h"
#include "btClosestBodyDetector.h"

PhysicsObject::PhysicsObject(btRigidBody* body, Vector3 position, BoundingBox bBox, Game::Material material)
{

	this->body = body;
	this->material = material;
	isMoving = false;

	speed = Vector3(0, 0, 0); //Velocity
	weight = std::numeric_limits<float>::infinity();
	invWeight = 0; //Inverse of weight

	this->boundingBox = bBox;

	positions.push_back(position);
	positions.push_back(position);
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	cout << transform.getRotation();
	rotation = new btQuaternion(transform.getRotation());
	initialRotation = new btQuaternion(transform.getRotation());
	vPos.current = Vector3(position);
	vPos.previous = Vector3(position);
	SetVerletPosition();
}

PhysicsObject::PhysicsObject(Vector3 position, BoundingBox bBox) : PhysicsObject(NULL, position, bBox, Game::Material::Metal)
{
}
PhysicsObject::PhysicsObject(const PhysicsObject& p)
{
	body = new btRigidBody(*p.body);
	type = p.type;
	material = p.material;
	time = p.time;
	weight = p.weight;
	invWeight = p.invWeight;
	aggroRange = p.aggroRange;
	isMoving = p.isMoving;

	speed = p.speed;
	positions = p.positions;
	rotation = p.rotation;
	initialRotation = p.initialRotation;
	damping = p.damping;

	collidesWith.resize(p.collidesWith.size());
	for (int i = 0; i < p.collidesWith.size(); i++)
	{
		switch (p.collidesWith[i]->GetType())
		{
		case Game::Type::Still:
		{
								  collidesWith[i] = new PlainObject(*(PlainObject*)p.collidesWith[i]);
								  break;
		}
		case Game::Type::Controllable:
		{
								 collidesWith[i] = new ControllableObject(*(ControllableObject*)p.collidesWith[i]);
								 break;
		}
		}
	}
}

PhysicsObject::~PhysicsObject()
{
}


void PhysicsObject::Update(float msec, set<Action> actions)
{
	if (body != NULL)
	{
		if (isMoving)
		{
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform);
			SetPosition(Vector3(&transform.getOrigin()));
			SetRotation(new btQuaternion(transform.getRotation()));
		}
	}
	/*if (IsMoving())
	{
		PhysicsContainer *physicsWorld = &PhysicsContainer::Instance();
		PhysicsObject* physics = this;
		physicsWorld->VerletDisplacement(physics, msec);
	}*/
}

void PhysicsObject::SetRotation(btQuaternion* quat)
{
	this->rotation = quat;
}
void PhysicsObject::SetPosition(const Vector3 &pos)
{
	Vector3 distance = vPos.current - vPos.previous;
	positions.push_back(pos);
	vPos.current = pos;
	vPos.previous = pos - distance;
}

Vector3 PhysicsObject::GetScale()
{
	return (boundingBox.frontTopRight - boundingBox.backBottomLeft);
}

void PhysicsObject::SetVerletPosition()
{
	vPos.current = positions.at(positions.size() - 1);
	vPos.previous = vPos.current - speed;
}

void PhysicsObject::Move()
{
	Vector3 newPosition = positions.at(positions.size() - 1) + speed;

	if (positions.size() == 10)
	{
		positions.erase(positions.begin());
	}
	positions.push_back(newPosition);
}

void PhysicsObject::Move(const float &period)
{
	Vector3 newPosition = positions.at(positions.size() - 1) + speed * period;

	if (positions.size() == 10)
	{
		positions.erase(positions.begin());
	}
	positions.push_back(newPosition);
}

void PhysicsObject::Move(Vector3 displacement)
{
	vPos.previous = vPos.current;
	vPos.current = displacement;
	if (positions.size() == 10)
	{
		positions.erase(positions.begin());
	}
	positions.push_back(displacement);

	UpdateBoundingBox(displacement - vPos.previous);
}

//Basically the same as Move(Vector3 displacement), except that the previous position of the object is set differently
void PhysicsObject::CollisionMove(Vector3 displacement)
{
	vPos.previous -= (vPos.current - displacement);
	UpdateBoundingBox(displacement - vPos.previous);
	vPos.current = displacement;
	if (positions.size() == 10)
	{
		positions.erase(positions.begin());
	}
	positions.push_back(displacement);
}

void PhysicsObject::Accelerate()
{
	speed += acceleration;
}

void PhysicsObject::SetWeight(const float &newWeight)
{
	weight = newWeight;
	invWeight = 1 / newWeight;
}

void  PhysicsObject::SetAcceleration(const Vector3 &accel)
{
	acceleration = accel;
}


void PhysicsObject::SetSpeed(const Vector3 &speed)
{
	this->speed = speed;
}

void PhysicsObject::Accelerate(const float &period)
{
	float coef = period * 0.001;
	speed += acceleration * coef;
}

void PhysicsObject::ApplyForce(const Vector3 &force)
{
	acceleration = force * invWeight;
}

void PhysicsObject::AddForce(const Vector3 &force)
{
	acceleration = acceleration + (force * invWeight);
}

Vector3 PhysicsObject::GetPosition()
{
	return positions.at(positions.size() - 1);
}

Vector3 PhysicsObject::GetPreviousPosition()
{
	if (positions.size() < 2)
		return GetPosition();
	return positions.at(positions.size() - 2);
}

bool PhysicsObject::IsOnGround()
{
	btTransform transform;
	btTransform dest;
	btVector3 s = body->getCollisionShape()->getLocalScaling();
	const btDiscreteDynamicsWorld* bw = PhysicsContainer::Instance().GetBulletDynamicsWorld();
	body->getMotionState()->getWorldTransform(transform);
	dest = transform;
	btQuaternion rot = body->getOrientation();
	transform.setOrigin(transform.getOrigin() - btVector3(s.x() * 0.5, 0, s.z() * 0.5));
	dest.setOrigin(transform.getOrigin() - btVector3(-s.x() * 0.5, s.y(), -s.z() * 0.5));
	btClosestBodyDetector cb(body, &transform.getOrigin(), &dest.getOrigin());
	cb.m_collisionFilterMask = btBroadphaseProxy::StaticFilter;
	if (body->getCollisionShape()->isConvex())
	{
		bw->convexSweepTest((btConvexShape*)body->getCollisionShape(), transform, dest, cb, 0.5);
		return cb.hasHit();
	}
	return false;
}


bool PhysicsObject::IsInsideShape(Vector3 point, vector<Vector3> edges, float radius)
{
	vector<Vector3>::iterator iter;
	double area = pow(sqrt(pow(edges[1].x - edges[0].x, 2) + pow(edges[1].y - edges[0].y, 2) + pow(edges[1].z - edges[0].z, 2) + 3 * pow(2 * radius, 2)), 2);
	double test = 0;
	for (iter = edges.begin(); iter < edges.end() - 1; iter++)
	{
		test += abs(triangleArea(*iter, *(iter + 1), point));
	}
	test += triangleArea(*iter, *edges.begin(), point);
	return ((test - area) < 0);
}

