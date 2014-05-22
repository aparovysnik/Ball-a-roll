#include "LevelResource.h"
#include "ResourceManager.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "EmbeddedResources.h"


LevelResource::LevelResource()
{
	//Shader 0
	shaders.push_back(ResourceManager::Instance().GetResource<ShaderResource>("testvert.glsl,testFrag.glsl")->GetShader());

	//Shader 1
	shaders.push_back(ResourceManager::Instance().GetResource<ShaderResource>("testvert.glsl,testFragTex.glsl")->GetShader());

	//Shader 2
	shaders.push_back(ResourceManager::Instance().GetResource<ShaderResource>("testVert.glsl,testFragTexCol.glsl")->GetShader());
}

LevelResource::LevelResource(const string& addr)
{
	Load(addr);
}

LevelResource::~LevelResource()
{
	entities.clear();
}

bool LevelResource::Load(const string& addr)
{
	vector <vector <string> > data;
	ifstream infile(addr);

	Mesh* mesh;
	btCollisionShape* col;
	Vector3 size;
	float mass;
	Vector3 rotation;
	Vector3 position;
	float restitution = 0, friction = 0, rollingFriction = 0, angDamping = 0;
	bool transparent = false;
	Game::Type type;
	bool isFinish;
	
	//
	//vector<void> v;
	string s;
	if ((bool)getline(infile, s))
	{
		while (infile)
		{
			if (!getline(infile, s))
			{
				cout << "Loaded level!" << endl;
				return true;
			}

			istringstream stream(s);
			vector <string> record;

			//Read and configure mesh type
			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			switch (atoi(s.c_str()))
			{
			case 0:
			{
					  mesh = Mesh::GenerateQuad();
					  break;
			}
			case 1:
			{
					  mesh = Mesh::GenerateSphere(1, 96, 192);
					  break;
			}
			}

			//Read bttype

			if (!GetValue(stream, s, addr))
			{
				return false;
			}
			string bttype = s;

			//Read size

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			size.x = atof(s.c_str());

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			size.y = atof(s.c_str());

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			size.z = atof(s.c_str());

			//Configure btshape
			if (bttype.compare("btbox") == 0)
				col = new btBoxShape(size.ToBtVector());
			else if (bttype.compare("btsphere") == 0)
				col = new btSphereShape(size.x);

			//Read mass


			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			mass = atof(s.c_str());

			//Read rotation

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			rotation.x = atof(s.c_str());

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			rotation.y = atof(s.c_str());

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			rotation.z = atof(s.c_str());

			//Read position

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			position.x = atof(s.c_str());

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			position.y = atof(s.c_str());

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			position.z = atof(s.c_str());

			//Read restitution

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			restitution = atof(s.c_str());

			//Read friction

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			friction = atof(s.c_str());

			//Read rolling friction

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			rollingFriction = atof(s.c_str());

			//Read angular damping

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			angDamping = atof(s.c_str());

			//Read transparency

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			transparent = atoi(s.c_str());

			//Read shader

			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			Shader* shader = shaders[atoi(s.c_str())];

			btVector3 fallInertia;
			mesh->AddLight(Vector4(255, 255, 255, 255), Vector3(0, 10, 10), 100);
			RenderObject* obj = new RenderObject(mesh, shader);

			//Read texture
			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			if (!s.empty())
			{
				TextureResource* tex = ResourceManager::Instance().GetResource<TextureResource>(s.c_str());
				if (tex != NULL)
					obj->SetTexture(tex->GetTextureID());
			}

			Matrix4 mMatrix = Matrix4::Translation(position) * 
				Matrix4::Rotation(rotation.x, Vector3(1, 0, 0)) *
				Matrix4::Rotation(rotation.y, Vector3(0, 1, 0)) *
				Matrix4::Rotation(rotation.z, Vector3(0, 0, 1)) *
				Matrix4::Scale(size);
			obj->SetModelMatrix(mMatrix);

			//Read finishing point configuration
			if (!GetValue(stream, s, addr))
			{
				return false;
			}
			isFinish = atoi(s.c_str());

			//Read entity type
			if (!GetValue(stream, s, addr))
			{
				return false;
			}

			switch (atoi(s.c_str()))
			{
			case 0:
			{
					  type = Game::Type::Still;
					  break;
			}
			case 1:
			{
					  type = Game::Type::Controllable;
					  break;
			}
			}

			//Convert rotation to radians
			rotation = rotation * (M_PI / 180);

			btVector3 inertia = btVector3(0, 0, 0);
			col->calculateLocalInertia(mass, inertia);
			btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(1, 0, 0), btScalar(rotation.x)) * btQuaternion(btVector3(0, 1, 0), btScalar(rotation.y)) * btQuaternion(btVector3(0, 0, 1), btScalar(rotation.z)), position.ToBtVector()));
			btRigidBody::btRigidBodyConstructionInfo
				rigidBodyCI(mass, motionState, col, inertia);
			rigidBodyCI.m_restitution = restitution;
			rigidBodyCI.m_friction = friction;

			rigidBodyCI.m_rollingFriction = rollingFriction;

			btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

			rigidBody->setAnisotropicFriction(col->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

			GameEntity* ent = new GameEntity(obj, rigidBody, Game::Material::Rubber, type);
			if (isFinish)
				ent->SetFinish();

			ent->SetTransparency(transparent);
			entities.push_back(new LevelEntity(ent, rigidBody, transparent));
		}
	}
	else
	{
		return false;
	}
	cout << "Loaded level!" << endl;
	return true;
}
bool LevelResource::GetValue(istringstream &stream, string& s, string addr)
{
	Logger::Instance().Write(LOG_CLIENT, (unsigned long)ID_INVALID_RESOURCE, addr.c_str());
	return ((bool)getline(stream, s, ','));
}

void LevelResource::Unload()
{

}
