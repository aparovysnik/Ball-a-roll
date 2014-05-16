#pragma once
#include "Resource.h"
#include "GameEntity.h"

struct LevelEntity
{
	LevelEntity(GameEntity* ent, btRigidBody* body, bool transparency)
	{
		this->entity = ent;
		this->rigidBody = body;
		this->isTransparent = transparency;
	}

	GameEntity* entity;
	btRigidBody* rigidBody;
	bool isTransparent;
};

class LevelResource : public Resource
{
public:
	LevelResource();
	LevelResource(const string& addr);

	~LevelResource();

	bool Load(const string& addr);
	void Unload();
	vector<LevelEntity*> GetLevelEntities() { return entities; }

private:

	bool GetValue(istringstream &stream, string& s, string addr);
	vector<LevelEntity*> entities;

	vector<Shader*> shaders;
};

