#include "ControllableObject.h"
#include "Renderer.h"
#include "ResourceManager.h"


ControllableObject::ControllableObject(btRigidBody* body, Vector3 position, BoundingBox bBox, Game::Material material) : PhysicsObject(body, position, bBox, material)
{
	isMoving = false;
	type = Game::Type::Controllable;
	body->setActivationState(2);
}

ControllableObject::~ControllableObject()
{
}

void ControllableObject::Update(float msec, set<Action> actions)
{
	bool moveStatusBg = isMoving;
	Renderer* r = &Renderer::Instance();
	if (actions.find(MOVE_CAM_FORWARD) != actions.end())
	{
		isMoving = true;
		//if (fwDir.Length() == 0)
			fwDir = r->GetCamDirection();
		body->setActivationState(1);
		body->applyCentralForce(btVector3(fwDir.x * 500, fwDir.y * 500, fwDir.z * 500));
	}
	else
	{
		fwDir = Vector3(0, 0, 0);
	}
	if (actions.find(MOVE_CAM_BACKWARD) != actions.end())
	{
		isMoving = true;
		//if (bwDir.Length() == 0)
			bwDir = r->GetCamDirection().Inverse();
		body->setActivationState(1);
		body->applyCentralForce(btVector3(bwDir.x * 500, bwDir.y * 500, bwDir.z * 500));
	}
	else
	{
		bwDir = Vector3(0, 0, 0);
	}
	if (actions.find(MOVE_CAM_LEFT) != actions.end())
	{
		isMoving = true;
		//if (lfDir.Length() == 0)
			lfDir = r->GetCamLeft().Inverse();
		body->setActivationState(1);
		body->applyCentralForce(btVector3(lfDir.x * 500, lfDir.y * 500, lfDir.z * 500));
	}
	else
	{
		lfDir = Vector3(0, 0, 0);
	}
	if (actions.find(MOVE_CAM_RIGHT) != actions.end())
	{
		isMoving = true;
		//if (rtDir.Length() == 0)
			rtDir = r->GetCamLeft();
		body->setActivationState(1);
		body->applyCentralForce(btVector3(rtDir.x * 500, rtDir.y * 500, rtDir.z * 500));
	}
	else
	{
		rtDir = Vector3(0, 0, 0);
	}
	if ((actions.find(JUMP) != actions.end()) && (IsOnGround()))
	{
		isMoving = true;
		if (!Controller::Instance().IsContinuous(JUMP))
		{
			//if (upDir.Length() == 0)
			upDir = r->GetCamUp();
			body->setActivationState(1);
			body->applyCentralForce(btVector3(upDir.x * 20000, upDir.y * 20000, upDir.z * 20000));
			ResourceManager::Instance().GetResource<SoundEffectResource>("jumping.wav")->Play();
		}
	}
	else
	{
		upDir = Vector3(0, 0, 0);
	}

	if (((GetPosition() - GetPreviousPosition()).Length() == 0) && !((moveStatusBg == false) && (isMoving == true)))
	{
		isMoving = false;
	}
	PhysicsObject::Update(msec, actions);
}
