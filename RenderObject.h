#pragma once

#include "../nclgl/Matrix4.h"
#include "Mesh.h"
#include "Shader.h"
#include "../nclgl/OGLRenderer.h"

class RenderObject	{
public:
	RenderObject(void);
	RenderObject(Mesh*m, Shader*s, GLuint t = 0);
	RenderObject(const RenderObject& r)
	{
		if (r.mesh != NULL)
			mesh = new Mesh(*r.mesh);
		else
			mesh = NULL;
		if (r.shader != NULL)
			shader = new Shader(*r.shader);
		else
			shader = NULL;
		texture = r.texture;
		modelMatrix = r.modelMatrix;
		initialMatrix = r.initialMatrix;
		isViewTransformable = r.isViewTransformable;
		worldTransform = r.worldTransform;
		if (r.parent == NULL)
			parent = NULL;
		for (int i = 0; i < r.children.size(); i++)
		{
			children.push_back(new RenderObject(*r.children[i]));
			children[i]->parent = this;
		}
		transparent = r.transparent;
	}

	~RenderObject(void);

	Mesh*	GetMesh()	const			{return mesh;}

	bool HasTexture()
	{
		return (texture != 0);
	}

	vector<Mesh*> GetMeshes()
	{
		vector<Mesh*> results;
		for each(RenderObject* child in children)
		{
			Mesh* m = child->GetMesh();
			if (m == NULL)
			{
				vector<Mesh*> childMeshes = child->GetMeshes();
				for each(Mesh* childMesh in childMeshes)
				{
					results.push_back(childMesh);
				}
			}
			else
			{
				results.push_back(m);
			}
		}
		return results;
	}
	void	SetMesh(Mesh*m)				{mesh = m;}

	Shader* GetShader()		const		{return shader; }
	void	SetShader(Shader*s)			{shader = s;}

	GLuint	GetTexture()		const	{return texture;}
	void	SetTexture(GLuint tex)		{texture = tex;}

	virtual void	SetModelMatrix(Matrix4 mat) { modelMatrix = mat; initialMatrix = mat; }

	void SetPosition(const Vector3& pos)
	{
		modelMatrix.SetPositionVector(pos);
	}

	void SetRotation(const Quaternion& quat)
	{
		Matrix4 newMatrix = (quat.ToMatrix() * initialMatrix);
		newMatrix.SetPositionVector(modelMatrix.GetPositionVector());
		modelMatrix = newMatrix;
	}

	Matrix4 GetModelMatrix()	const	{return modelMatrix;}


	virtual void Update(float msec);

	virtual void Draw() const;
	virtual void ButtonClicked(int id) {};

	//virtual void Update(float msec);

	void	AddChild(RenderObject* child) {
		children.push_back(child);
		child->parent = this;
	}

	void	AddChild(RenderObject* child, int position)
	{
		children.insert(children.begin() + (position), child);
		child->parent = this;
	}

	Matrix4 GetWorldTransform() const {
		return worldTransform;
	}

	const vector<RenderObject*>& GetChildren() const  {
		return children;
	}

	virtual bool CollidesWith(RenderObject* obj);
	void SetTransparent(bool choice) { transparent = choice; }
	bool IsTransparent() const { return transparent; }

	void ToggleViewTransformation(bool choice)
	{
		isViewTransformable = choice;
	}

	bool IsTransformable() const
	{
		return isViewTransformable;
	}

protected:
	Mesh*	mesh;
	Shader*	shader;

	GLuint	texture;

	Matrix4 modelMatrix;
	Matrix4 initialMatrix;
	Matrix4 worldTransform;

	bool isViewTransformable;

	Matrix4 RetrieveProjectionMatrix();

	RenderObject*			parent;
	vector<RenderObject*>	children;
	bool transparent;

	void ActivateLighting() const;
};

