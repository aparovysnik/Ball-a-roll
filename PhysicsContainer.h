#pragma once
#include "nclgl/Vector2.h"
#include "nclgl/Vector3.h"
#include "Timer.h"
#include "Singleton.h"
#include "Environment.h"
#include "btBulletDynamicsCommon.h"


class PhysicsContainer : public Singleton<PhysicsContainer>
{
	friend class Singleton<PhysicsContainer>;
	friend class State;
public:

	void UpdateScene(float msec);

	void SetEnvironment(Environment* env) { this->environment = env; };
	static void Create(Environment* env) { if (instance == NULL) { instance = new PhysicsContainer(env); } };

	Vector3 CalculateSpeed(PhysicsObject* &entity, const float &period);
	Vector3 CalculateDisplacement(PhysicsObject* &entity, const float &period);

	void Euler(PhysicsObject* &entity, const float &period);
	void Euler(PhysicsObject* &entity, const Vector3 &newAcceleration, const float &period);
	void SymplecticEuler(PhysicsObject* &entity, const float &period);


	void ResolveCollisions(GameEntity* entity, bool respond, float msec);
	void VerletDisplacement(PhysicsObject* &entity, const float &period);

	void ApplyGravity(const Vector3& g);
	Vector3 GetGravity() { return gravity; }

	bool ContainsCamObject()
 	{
		return containsCamObject;
	}

	//Retrieves the current velocity of the object bound to the camera
	Vector3 GetCamObjectVelocity()
	{
		return camObject->GetMovement();
	}

	Vector3 GetCamObjectOrigin()
	{
		if (camObject == NULL)
			return Vector3(0, 0, 0);
		return camObject->GetPosition();
	}

	btDiscreteDynamicsWorld* GetBulletDynamicsWorld()
	{
		return environment->GetBulletDynamicsWorld();
	}

	ControllableObject* GetCameraObject()
	{
		return camObject;
	}

	virtual void RespondToCollision(Vector3 normal, PhysicsObject* o1, PhysicsObject* o2, float dist, float msec);

	//bool ContainsPhysics(PhysicsObject &p) { return(std::find(physics.begin(), physics.end(), p) != physics.end()); }

protected:
	Environment* environment;
	PhysicsContainer(Environment* environment);
	~PhysicsContainer();

	Environment* GetEnvironment() { return environment; }

	static const map<Game::Material, float> coefficients;
	Vector3 gravity;

	ControllableObject* camObject;
	
	bool containsCamObject;
};

