#include "GameOnState.h"
#include "ResourceManager.h"
#include "ControllableObject.h"
#include "GameApplication.h"


GameOnState::GameOnState(string mName, string mDescription) : State(mName, mDescription)
{
	physics = &PhysicsContainer::Instance();
	renderer = &Renderer::Instance();
	mus = NULL;
	environment = NULL;

	Init();
}


GameOnState::~GameOnState()
{
}

void GameOnState::SetRunning(bool reinit)
{
	State::SetRunning(reinit);

	//Play or resume music
	if (mus == NULL || reinit)
	{
		mus = ResourceManager::Instance().GetResource<MusicResource>("Andrea Barone - Its like a Movie.mp3");
		mus->Play();
	}
	else
	{
		mus->Resume();
	}
	GameApplication::Instance().SetEnvironment(GetName());

	//Activate controller actions
	Controller* controller = &Controller::Instance();
	controller->ActivateAction(MOVE_CAM_BACKWARD, GLOBAL);
	controller->ActivateAction(MOVE_CAM_FORWARD, GLOBAL);
	controller->ActivateAction(MOVE_CAM_LEFT, LOCAL);
	controller->ActivateAction(MOVE_CAM_RIGHT, LOCAL);
	controller->ActivateAction(MOVE_CAM_UP, GLOBAL);
	controller->ActivateAction(MOVE_CAM_DOWN, GLOBAL);
	controller->ActivateAction(TURN_CAM_LEFT, GLOBAL);
	controller->ActivateAction(TURN_CAM_RIGHT, GLOBAL);
	controller->ActivateAction(MOVE_CAM_BACKWARD, LOCAL);
	controller->ActivateAction(MOVE_CAM_FORWARD, LOCAL);
	controller->ActivateAction(JUMP, LOCAL);
	controller->ActivateAction(PAUSE, LOCAL);
	controller->ActivateAction(RESTART, LOCAL);
}

void GameOnState::Run()
{

}

void GameOnState::Stop()
{
	mus->Stop();
}

void GameOnState::Init()
{
	if (environment != NULL)
		delete environment;

	this->environment = GameApplication::Instance().CreateEnvironment(mName);
	
	ResourceManager::Instance().FreeResource("level1.txt");
	LevelResource* level = ResourceManager::Instance().GetResource<LevelResource>("level1.txt");
	vector<LevelEntity*> entities = level->GetLevelEntities();
	
	for each (LevelEntity* ent in entities)
	{
		environment->AddEntity(ent->entity, ent->isTransparent);
		environment->AddBody(ent->rigidBody);
	}

	State::Init();

	/*Sample environment*/

	/*TextureResource* ballTex = ResourceManager::Instance().GetResource<TextureResource>("mayan.png");
	Shader* s1 = new Shader("testvert.glsl", "testFrag.glsl");
	Shader* s2 = new Shader("testvert.glsl", "testFragTex.glsl");
	Shader* s3 = new Shader("testvert.glsl", "testFragTexCol.glsl");
	Matrix4 mMatrix;

	btCollisionShape* groundShape = new btBoxShape(btVector3(30, 30, 0));

	btCollisionShape* fallShape = new btSphereShape(2);


	btDefaultMotionState* fallMotionState;
	btScalar mass = 0;
	btVector3 fallInertia;

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(1, 0, 0), btScalar(M_PI * 0.5)) * btQuaternion(btVector3(0, 1, 0), btScalar(M_PI * 0.1)), btVector3(0, -10, -70)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.9f;
	groundRigidBodyCI.m_friction = 1.0f;
	groundRigidBodyCI.m_rollingFriction = 1.0f;
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	//Generate quad
	Mesh* quadMesh = Mesh::GenerateQuad();
	//Add plain
	RenderObject* plain = new RenderObject(quadMesh, s1);
	mMatrix = Matrix4::Translation(Vector3(0, -10, -70)) * Matrix4::Scale(Vector3(30, 30, 0));
	plain->SetModelMatrix(mMatrix);

	GameEntity* entity0 = new GameEntity(plain, groundRigidBody, Game::Material::Rubber, Game::Type::Still);
	entity0->SetTransparency(true);
	environment->AddEntity(entity0, true);
	environment->AddBody(groundRigidBody);

	//Add sphere
	RenderObject* sphere1 = new RenderObject();
	Mesh* sphereMesh = Mesh::GenerateSphere(1, 96, 192);
	sphere1->SetMesh(sphereMesh);
	sphere1->SetShader(s3);
	sphere1->SetTexture(ballTex->GetTextureID());
	mMatrix = Matrix4::Translation(Vector3(0, -8, -50)) * Matrix4::Scale(Vector3(2, 2, 2));
	sphere1->SetModelMatrix(mMatrix);

	//Construct rigid body
	btTransform tr(btQuaternion(0, 0, 0, 1), btVector3(0, -8, -50));
	fallMotionState = new btDefaultMotionState(tr);
	mass = 10;
	fallInertia = btVector3(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBodyCI.m_rollingFriction = 0.3f;
	fallRigidBodyCI.m_friction = 1.0f;
	fallRigidBodyCI.m_restitution = 0.5f;
	fallRigidBodyCI.m_angularDamping = 0.5f;
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);

	fallRigidBody->setAnisotropicFriction(fallShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

	environment->AddBody(fallRigidBody);

	PhysicsObject* spherePhysics1 = new ControllableObject(fallRigidBody, mMatrix.GetPositionVector(), sphereMesh->CalculateBoundingBox(mMatrix),
		Game::Material::Metal);
	spherePhysics1->ActivateMovement();
	spherePhysics1->SetType(Game::Type::Controllable);
	spherePhysics1->SetWeight(10);

	GameEntity* entity1 = new GameEntity(sphere1, spherePhysics1);
	environment->AddEntity(entity1, false);*/
}
