#include "GameEntity.h"
#include "PlainObject.h"
#include "ControllableObject.h"
#include "nclgl/Matrix3.h"

void GameEntity::AssignPhysics(btRigidBody* body, Vector3 position, Game::Material material, Game::Type type)
{
	Matrix4 mMatrix = GetVisuals()->GetModelMatrix();
	switch (type)
	{
		case Game::Type::Still:
		{
			RenderObject* v = GetVisuals();
			Mesh* m = v->GetMesh();
			Vector3 normal = m->GetNormals()[0];
			normal = (Matrix3(v->GetModelMatrix()) * normal);
			normal.Normalise();
			BoundingBox bBox = m->CalculateBoundingBox(mMatrix);
			vector<Vector3> edgePoints = m->GetEdgePoints(mMatrix);
			physics = new PlainObject(body, position, edgePoints, bBox, normal, material);
			break;
		}
		case Game::Type::Controllable:
		{
			BoundingBox bBox = GetVisuals()->GetMesh()->CalculateBoundingBox(mMatrix);
			physics = new ControllableObject(body, position, bBox, material);
			break;
		}
	}
}
