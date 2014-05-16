#pragma once
#include "RenderObject.h"
#include "Controller.h"
#include "PhysicsObject.h"
#include "PlainObject.h"
#include "ControllableObject.h"
#include "TextObject.h"
class GameEntity
{
public:
	friend class Renderer;

	//Create an empty container
	GameEntity()
	{
		visuals = NULL;
		physics = NULL;
		finish = false;
		oneFrameOnly = false;
		isVisible = true;
		isTransparent = false;
	}

	GameEntity(const GameEntity &ent)
	{
		finish = ent.finish;
		visuals = new RenderObject(*ent.visuals);
		if (ent.physics == NULL)
			physics = NULL;
		else
		{
			switch (ent.physics->GetType())
			{
			case Game::Type::Controllable:
			{
											 physics = new ControllableObject(*(ControllableObject*)ent.physics);
											 break;
			}
			case Game::Type::Still:
			{
									  physics = new PlainObject(*(PlainObject*)ent.physics);
									  break;
			}
			default:
				cout << "Invalid type!" << ent.physics->GetType() << endl;
			}
		}
		oneFrameOnly = ent.oneFrameOnly;
		isTransparent = ent.isTransparent;
		isVisible = ent.isVisible;
		children.resize(ent.children.size());
		for (int i = 0; i < ent.children.size(); i++)
		{
			children[i] = new GameEntity(*ent.children[i]);
		}
	}

	GameEntity(btRigidBody* body, Vector3 position, Vector3 scale, Game::Material material, Game::Type type)
	{
		finish = false;
		oneFrameOnly = false;
		isVisible = true;
		isTransparent = false;

		AssignPhysics(body, position, material, type);
	}

	GameEntity(RenderObject* visuals, btRigidBody* body, Game::Material material, Game::Type type)
	{
		finish = false;
		SetVisuals(visuals);
		oneFrameOnly = false;
		isVisible = true;
		isTransparent = false;

		AssignPhysics(body,visuals->GetModelMatrix().GetPositionVector(), material, type);
	}


	GameEntity(RenderObject* visuals)
	{
		finish = false;
		SetVisuals(visuals);
		oneFrameOnly = false;
		isVisible = true;
		isTransparent = false;
		
		
		Mesh* m = visuals->GetMesh();
		if (m == NULL)
		{
			physics = NULL;
			//physics = new TextObject(data.position, Mesh::CalculateBoundingBox(visuals->GetMeshes(), visuals->GetModelMatrix()));
		}
		else
		{
			physics = NULL;
			//physics = new TextObject(data.position, visuals->GetMesh()->CalculateBoundingBox(visuals->GetModelMatrix()));
		}
	}

	GameEntity(RenderObject* visuals, PhysicsObject* physics)
	{
		finish = false;
		SetVisuals(visuals);
		this->physics = physics;
		memcpy(this->physics, physics, sizeof(physics));
		oneFrameOnly = false;
		isVisible = true;
		isTransparent = false;
	}

	~GameEntity()
	{
		children.clear();
		if (visuals != NULL)
			delete visuals;
	}

	bool IsMoving() {
		if (physics != NULL)
			return (physics->IsMoving());
		return false;
	}

	void SetVisuals(RenderObject* obj)
	{
		this->visuals = obj;
	}

	void SetFinish() { finish = true; }
	bool IsFinish() { return finish; }

	bool HasVisuals() { return visuals != NULL; }
	void SetVisibility(bool vis) { isVisible = vis; }
	bool IsVisible() { return isVisible; }

	//Update object
	virtual void Update(float msec, set<Action>actions) {

		if (physics != NULL)
		{
			physics->Update(msec, actions);
			Sync();
		};
		if (visuals != NULL)
		{
			visuals->Update(msec);
		}
		for each (GameEntity* child in children)
		{
			child->Update(msec, actions);
		}
	}

	virtual void UpdatePhysics(float msec, set<Action>actions)
	{
		if (physics != NULL)
		{
			physics->Update(msec, actions);
			Sync();
		}

		for each (GameEntity* child in children)
		{
			child->Update(msec, actions);
		}
	}

	virtual void UpdateVisuals(float msec)
	{
		if (visuals != NULL)
		{
			visuals->Update(msec);
		}

		for each (GameEntity* child in children)
		{
			child->UpdateVisuals(msec);
		}
	}

	RenderObject* GetVisuals() { return visuals; }
	PhysicsObject* GetPhysics() { return physics; }
	virtual void Sync()
	{
		visuals->SetPosition(visuals->GetModelMatrix().GetPositionVector() + (physics->GetPosition() - physics->GetPreviousPosition()));
		visuals->SetRotation(Quaternion(physics->GetRotation()));
	}

	void AddChild(GameEntity* ent) { children.push_back(ent); };

	void RemoveChildren() { children.clear(); }

	void RemoveChild(GameEntity* ent)
	{
		std::vector<GameEntity*>::iterator iter = std::find(children.begin(), children.end(), ent);
		if (iter != children.end())
			children.erase(iter);
	}

	vector<GameEntity*> GetChildren() { return children; }

	void DeleteAfterRendering(bool choice){ oneFrameOnly = choice; }
	bool IsOneFrameOnly() { return oneFrameOnly; }
	void SetTransparency(bool t) { isTransparent = t; }
	bool IsTransparent() { return isTransparent; }

protected:
	bool isVisible;
	RenderObject* visuals; //Visual representation of the object
	PhysicsObject* physics; //Object physics

	vector<GameEntity*> children;
	bool oneFrameOnly;
	bool isTransparent;
	bool finish;

	void AssignPhysics(btRigidBody* body, Vector3 position, Game::Material material, Game::Type type);
};

