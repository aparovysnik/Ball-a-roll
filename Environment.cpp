#include "Environment.h"
#include "GameEntity.h"


Environment::~Environment()
{
	delete dynamicsWorld;
	delete octree;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	for each (GameEntity* ent in entities)
	{
		delete ent;
	}
}

void Environment::AddEntity(GameEntity* ent, bool transparent)
{
	if (!ContainsEntity(ent))
	{
		entities.push_back(ent);
		//octree->Insert(ent);

		if (ent->IsFinish())
		{
			finishes.push_back(ent);
		}
	}
}

vector<GameEntity*> Environment::GetEntities()
{
	return entities;
}

void Environment::RemoveEntity()
{
	octree->Remove(entities.at(index - 1));
	delete entities.at(index - 1);
	entities.erase(entities.begin() + (index - 1));
}
void Environment::RemoveEntity(GameEntity* &ent)
{
	if (ent == NULL)
		return;
	delete ent;
	std::vector<GameEntity*>::iterator iter = std::find(entities.begin(), entities.end(), ent);
	if (iter != entities.end())
		entities.erase(iter);
	
}

void Environment::Update(float msec, set<Action> actions)
{
	for each(GameEntity* entity in entities)
	{
		entity->Update(msec, actions);
	}
}