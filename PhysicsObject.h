#pragma once
#include "../nclgl/Vector2.h"
#include "../nclgl/Vector3.h"
#include "RenderObject.h"
#include "ObjectTypes.h"
#include "Timer.h"
#include "BoundingBox.h"
#include "Collision.h"
#include <memory>
#include "btBulletDynamicsCommon.h"
#include "Controller.h"
#include "Math.h"

class PhysicsObject
{

public:
	PhysicsObject(btRigidBody* body, Vector3 position, BoundingBox bBox, Game::Material material);
	PhysicsObject(Vector3 position, BoundingBox bBox);

	PhysicsObject(const PhysicsObject& p);
	virtual ~PhysicsObject();
	virtual std::unique_ptr<PhysicsObject> Clone() = 0;

	struct VerletPosition
	{
		Vector3 current;
		Vector3 previous;
	};

	void SetType(Game::Type t)
	{
		this->type = t;
	}

	virtual void Update(float msec, set<Action>actions);
	Game::Type GetType() { return type; }
	Game::Material GetMaterial(){ return material; }

	void Move();
	void Move(const float &period);
	void Move(Vector3 displacement);

	void SetWeight(const float &newWeight);
	void SetAcceleration(const Vector3& accel);
	void SetSpeed(const Vector3& speed);
	void SetVerletPosition();

	Vector3 GetSpeed() { return speed; }
	Vector3 GetAcceleration() { return acceleration; }
	float GetWeight() { return weight; }

	bool IsMoving(){ return isMoving; }
	void ActivateMovement() { isMoving = true; }
	void DeactivateMovement() { isMoving = false; }


	void Accelerate();
	void Accelerate(const float& period);

	void ApplyForce(const Vector3 &force);
	void AddForce(const Vector3 &force);

	void SetPosition(const Vector3& pos);
	void SetRotation(btQuaternion* quat);

	btQuaternion* GetRotation() { return new btQuaternion((*rotation) * (*initialRotation).inverse()); }

	void SetScale(const Vector3 &scal);
	BoundingBox GetBoundingBox() { return boundingBox; };
	Vector3 GetScale();

	Vector3 GetPosition();
	Vector3 GetPreviousPosition();

	VerletPosition GetVerletPosition() { return vPos; }
	Vector3 GetVerletSpeed() { return vPos.current - vPos.previous; }
	void SetVerletSpeed(Vector3 velocity)
	{
		vPos.previous = vPos.current - velocity;
	}

	void SetDamping(float dmp)
	{
		if (dmp >= 0 && dmp <= 1)
		{
			damping = dmp;
		}
	}

	//Specifies whether the object is colliding with given PhysicsObject on current frame
	bool CollisionRecorded(PhysicsObject* obj)
	{
		return (std::find(collidesWith.begin(), collidesWith.end(), obj) != collidesWith.end());
	}
	//Specifies whether there is collision between two physics objects. Checks both of them.
	bool InCollision(PhysicsObject* obj)
	{
		bool test1 = CollisionRecorded(obj);
		bool test2 = obj->CollisionRecorded(this);
		return CollisionRecorded(obj) && obj->CollisionRecorded(this);
	}

	void RemoveCollision(PhysicsObject* obj)
	{
		vector<PhysicsObject*>::iterator iter = std::find(collidesWith.begin(), collidesWith.end(), obj);
		
		if(iter != collidesWith.end())
			collidesWith.erase(iter);
	}

	vector<PhysicsObject*> GetCollisions()
	{
		return collidesWith;
	}

	bool IsOnGround();

	virtual Game::CollisionSettings* CollidesWith(PhysicsObject* obj) { return NULL; }
protected:
	btRigidBody* body;
	Game::Type type;
	Game::Material material;
	Timer time;
	float weight;
	float invWeight; //Inverse of the weight for faster calculations
	float aggroRange;

	bool isMoving;

	Vector3 speed;
	Vector3 acceleration;

	VerletPosition vPos;
	BoundingBox boundingBox;
	vector<Vector3> positions;
	btQuaternion* rotation;
	btQuaternion* initialRotation;
	float damping;

	void CollisionMove(Vector3 displacement);
	vector<PhysicsObject*> collidesWith;
	bool IsInsideShape(Vector3 point, vector<Vector3> edges, float radius);
	void UpdateBoundingBox(Vector3 distance)
	{
		boundingBox.backBottomLeft += distance;
		boundingBox.frontTopRight += distance;
	}

	double triangleArea(Vector3 A, Vector3 B, Vector3 C)
	{
		double test2 = (Vector3::Cross((B - A), (A - C))).Length() * 0.5;
		return test2;
	}
};

