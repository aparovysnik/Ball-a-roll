#pragma once
#include "../nclgl/OGLRenderer.h"

#include "RenderObject.h"
#include "Controller.h"
#include "Singleton.h"
#include "Environment.h"
#include "SOIL.h"

#include <vector>


using std::vector;

class Renderer : public OGLRenderer, public Singleton<Renderer>	{

	friend class Singleton<Renderer>;
	friend class RenderObject;
public:

	virtual void	RenderScene();

	virtual void	Render(const RenderObject &o);

	virtual void	UpdateScene(float msec);

	void	AddRenderObject(RenderObject &r) {
		renderObjects.push_back(&r);
	}

	void	RemoveRenderObjects() {
		renderObjects.clear();
	}

	Vector2	GetRenderArea();
	Vector3 GetCamLeft() { return viewMatrix.GetCamLeftVector(); }
	Vector3 GetCamUp() { return viewMatrix.GetCamUpVector(); }
	Vector3 GetCamDirection() { return viewMatrix.GetForwardVector(); }
	Vector3 GetCamPosition() { return modelMatrix.GetPositionVector(); }

	void SetCameraOrigin(Vector3 newOrigin)
	{
		camOrigin = newOrigin;
	}

	void SetCameraMovement(Vector3 camVelocity)
	{
		camMovement = camVelocity;
	}

	void RotateCamera(float rate)
	{
		camRotation = rate;
	}

	GLuint LoadTexture(string filename);
	static void Create(Window w, Environment* env) { if (instance == NULL) { instance = new Renderer(w, env); } };
	void SetEnvironment(Environment* env) { this->environment = env; };

protected:

	vector<RenderObject*> renderObjects;
	GLuint tex;

	const float CAM_MOVEMENT_RATE;
	const float INV_CAM_MOVEMENT_RATE;
	const float ROTATION_RATE;
	const float INV_ROTATION_RATE;

	virtual void RenderEntity(GameEntity *ent);

	Environment* environment;

	Vector3 camOrigin;
	Vector3 camMovement;

	Matrix4 orthoProj;
	Matrix4 orthoView;

	float camRotation;

	Renderer(Window &parent, Environment* env);
	~Renderer(void);

private:
	//Gets projection matrix for the specified render object. Is accessible in RenderObject hierarchy.
	Matrix4 GetViewProjectionMatrix(RenderObject* r)
	{ 
		if (r->IsTransformable())
			return viewMatrix * projMatrix;
		else
			return orthoView * orthoProj;
	}

};

