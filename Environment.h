#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "GameEntity.h"
#include <list>
#include <algorithm>
#include "Octree.h"
#include "btBulletDynamicsCommon.h"
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace std;

class Environment
{
public:
	Environment(unsigned int maxCapacity, Vector3 origin, Vector3 halfDimension)
	{ 
		broadphase = new btDbvtBroadphase();
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
		solver = new btSequentialImpulseConstraintSolver();
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0, -50, 0));
		this->origin = origin;
		this->halfDim = halfDimension;
		this->maxCapacity = maxCapacity;
		octree = new Octree(origin, halfDimension);
	};

	Environment(const Environment &e)
	{
		broadphase = new btDbvtBroadphase();
		collisionConfiguration = new btDefaultCollisionConfiguration(*(btDefaultCollisionConfiguration*)e.collisionConfiguration);
		dispatcher = new btCollisionDispatcher(*e.dispatcher);
		btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
		solver = new btSequentialImpulseConstraintSolver(*e.solver);
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		octree = new Octree(*e.octree);
		entities.resize(e.entities.size());
		for (int i = 0; i < e.entities.size(); i++)
		{
			entities[i] = new GameEntity(*e.entities[i]);
		}
	}
	~Environment();

	void begin() { index = 0; }
	void end() { index = entities.size() - 1; }

	void AddEntity(GameEntity* ent, bool transparent);
	void RemoveEntity(GameEntity* &ent);
	void RemoveEntity();

	void RemoveEntityAt(unsigned int index)
	{
		if (index < entities.size())
		{
			entities.erase(entities.begin() + index);
		}
	}

	GameEntity* GetEntityAt(unsigned int ind)
	{
		if (ind < entities.size())
		{
			return entities.at(ind);
		}
		return NULL;
	}

	btDiscreteDynamicsWorld* GetBulletDynamicsWorld()
	{
		return dynamicsWorld;
	}

	void RemoveBody(btRigidBody* body) {
		this->dynamicsWorld->removeRigidBody(body);
	}

	void RemoveBodies() {
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	}

	Octree* GetOctree()
	{
		return octree;
	}
	GameEntity* GetNextEntity()
	{
		if (index < entities.size())
		{
			index++;
			return entities.at(index - 1);
		}
		return NULL;
	}
	void AddBody(btRigidBody* body)
	{
		this->dynamicsWorld->addRigidBody(body);
	}

	GameEntity* GetPreviousEntity()
	{
		if (index > 0)
		{
			index--;
			return entities.at(index + 1);
		}
		return NULL;
	}

	void Update(float msec, set<Action> actions);

	inline bool ContainsEntity(const GameEntity* obj) { return (std::find(entities.begin(), entities.end(), obj) != entities.end()); }

	vector<GameEntity*> GetEntities();

	vector<GameEntity*> GetTransparentEntities()
	{
		vector<GameEntity*> transparent;
		for each (GameEntity* ent in entities)
		{
			if (ent->IsTransparent())
			{
				transparent.push_back(ent);
			}
		}
		return transparent;
	}

	vector<GameEntity*> GetOpaqueEntities()
	{
		vector<GameEntity*> opaque;
		for each (GameEntity* ent in entities)
		{
			if (!ent->IsTransparent())
			{
				opaque.push_back(ent);
			}
		}
		return opaque;
	}

	vector<GameEntity*> GetFinishingPoints()
	{
		return finishes;
	}

	Vector3 GetHalfDimension() { return halfDim; }
	Vector3 GetOrigin() { return origin; }
	unsigned int GetCapacity() { return maxCapacity; }

private:
	unsigned int index;
	unsigned int maxCapacity;

	vector<GameEntity*> finishes;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	vector<GameEntity*> entities;
	btDiscreteDynamicsWorld* dynamicsWorld;
	Vector3 origin;
	Vector3 halfDim;
	Octree* octree;
};

