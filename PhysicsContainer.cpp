#include "PhysicsContainer.h"
#include "ControllableObject.h"

const map<Game::Material, float> PhysicsContainer::coefficients = map<Game::Material, float> {
	pair<Game::Material, float>(Game::Material::Metal, 0.9),
		pair<Game::Material, float>(Game::Material::Rubber, 0.9)};

PhysicsContainer::PhysicsContainer(Environment* env)
{
	camObject = NULL;
	containsCamObject = false;
	//dynamicsWorld->(btVector3(0, -100, 0));
	this->environment = env;
}


PhysicsContainer::~PhysicsContainer()
{
	delete camObject;
	delete environment;
}

void PhysicsContainer::UpdateScene(float msec)
{
	btDynamicsWorld* dynamicsWorld = environment->GetBulletDynamicsWorld();

	//Timer t;
	//t.StartTimer();
	dynamicsWorld->stepSimulation(1 / 60.f, 10);
	environment->begin();
	GameEntity *entity = environment->GetNextEntity();
	set<Action> objectActions = Controller::Instance().GetLocalActions();
	bool camObjectPresent = false;
	while (entity != NULL)
	{
		PhysicsObject* p = entity->GetPhysics();
		if (p != NULL)
		{
			entity->UpdatePhysics(msec, objectActions);

			if (p->GetType() == Game::Type::Controllable) {
				ControllableObject* ctr = dynamic_cast<ControllableObject*>(p);
				camObjectPresent = true;
				containsCamObject = true;
				camObject = ctr;
			}
		}
		entity = environment->GetNextEntity();
	}

	if (!camObjectPresent) {
		containsCamObject = false;
	}
	/*environment->begin();
	entity = environment->GetNextEntity();
	while (entity != NULL)
	{
		if (entity->IsMoving())
			ResolveCollisions(entity, true, msec);
		entity = environment->GetNextEntity();
	}*/
	/*environment->end();
	entity = environment->GetPreviousEntity();
	while (entity != NULL)
	{
	entity->Update(msec);
	if (entity->IsMoving())
	ResolveCollisions(entity, false);
	entity = environment->GetPreviousEntity();
	}*/
}

void PhysicsContainer::ResolveCollisions(GameEntity* entity, bool respond, float msec)
{
	struct CollisionData
	{
		CollisionData(Vector3 n, PhysicsObject* ent, PhysicsObject* obj, float p)
		{
			entity = ent;
			object = obj;
			normal = n;
			penetration = p;
		}
		PhysicsObject* entity;
		PhysicsObject* object;
		Vector3 normal;
		float penetration;
	};

	vector<CollisionData> collisions;

	Octree* octree = environment->GetOctree();
	octree->Update(entity);
	vector<GameEntity*> candidates;
	octree->GetPotentialCollisions(entity, candidates);
	Vector3 normal;
	for each(GameEntity* candidate in candidates)
	{
		/*if (candidate->GetPhysics()->CollisionRecorded(entity->GetPhysics()))
		continue;*/
		Game::CollisionSettings* collision = entity->GetPhysics()->CollidesWith(candidate->GetPhysics());
		if (respond && (collision != NULL))
		{

			RespondToCollision(collision->normal, entity->GetPhysics(), candidate->GetPhysics(), collision->distance, msec);
			collisions.push_back(CollisionData(normal, entity->GetPhysics(), candidate->GetPhysics(), collision->distance));
			/*for each (PhysicsObject* o in entity->GetPhysics()->GetCollisions())
			{
			if (o != candidate->GetPhysics())
			{
			candidate->GetPhysics()->CollidesWith(o);
			}
			}

			for each (PhysicsObject* o in candidate->GetPhysics()->GetCollisions())
			{
			if (o != entity->GetPhysics())
			{
			entity->GetPhysics()->CollidesWith(o);
			}
			}*/
			octree->Update(candidate);
		}
		else
		{
			entity->GetPhysics()->RemoveCollision(candidate->GetPhysics());
			candidate->GetPhysics()->RemoveCollision(entity->GetPhysics());
		}

		octree->Update(entity);
		for each (CollisionData col in collisions)
		{

		}
		delete collision;
	}
}
void PhysicsContainer::Euler(PhysicsObject* &entity, const float &period)
{
	entity->Move(period);
	entity->SetSpeed(entity->GetSpeed() + entity->GetAcceleration() * period);
}

void PhysicsContainer::Euler(PhysicsObject* &entity, const Vector3 &newAcceleration, const float & period)
{
	entity->SetSpeed(entity->GetSpeed() + entity->GetAcceleration() * period);
	entity->Move(period);
}

void PhysicsContainer::SymplecticEuler(PhysicsObject* &entity, const float& period)
{
	entity->SetSpeed(entity->GetSpeed() + entity->GetAcceleration() * period);
	entity->Move(period);
}

Vector3 PhysicsContainer::CalculateSpeed(PhysicsObject* &entity, const float &period)
{
	return (entity->GetSpeed() + entity->GetAcceleration() * period);
}

Vector3 PhysicsContainer::CalculateDisplacement(PhysicsObject* &entity, const float &period)
{
	return ((entity->GetSpeed() * period) + (entity->GetAcceleration() * period * period * 0.5f));
}

void PhysicsContainer::VerletDisplacement(PhysicsObject* &entity, const float &period)
{
	float coef;
	coef = period / 100;
	PhysicsObject::VerletPosition vPos = entity->GetVerletPosition();
	Vector3 test = entity->GetAcceleration() * (coef * coef);
	Vector3 displacement = (vPos.current * 2 - vPos.previous) + ((entity->GetAcceleration() + (GetGravity() / entity->GetWeight()))  * (coef * coef));
	entity->Move(displacement);
}

void PhysicsContainer::RespondToCollision(Vector3 normal, PhysicsObject* o1, PhysicsObject* o2, float pDepth, float msec)
{
	//normal.Normalise();
	Vector3 o1speed = o1->GetVerletSpeed();
	Vector3 o2speed = o2->GetVerletSpeed();

	Vector3 speed1;
	Vector3 speed2;

	float m1 = o1->GetWeight();
	float m2 = o2->GetWeight();

	float coef = msec * 0.01;
	Vector3 elimGravity1 = o1speed - (gravity / m1) * coef * coef;
	elimGravity1.Normalise();
	Vector3 elimGravity2 = o2speed - (gravity / m2) * coef * coef;
	elimGravity2.Normalise();
	float direction = Vector3::Dot(elimGravity1,elimGravity2);
	if (o1->IsMoving() && o2->IsMoving() && (abs(direction + elimGravity1.Length()) > 0.005))
	{
		speed1 = o2speed;
		speed2 = o1speed;
	}
	else
	{
		// substracting velocities of colliding objects to get overall velocity
		Vector3 sumVelocity;
		sumVelocity = o1->GetVerletSpeed() - o2->GetVerletSpeed();
		sumVelocity = sumVelocity * Vector3((normal.x), (normal.y), (normal.z));

		//Calculating damping / boosting value using materials assigned to objects
		//float test = coefficients.find(o1->GetMaterial())->second;
		float damping = coefficients.find(o1->GetMaterial())->second * coefficients.find(o2->GetMaterial())->second;

		//Calculating momentum
		Vector3 momentum = (normal * sumVelocity * (-1 - damping)) / (normal * normal * ((1 / m1) + (1 / m2)));
		float test1 = o1speed.Length();
		float test2 = ((gravity / m1) * coef * coef).Length();
		float test3 = o2speed.Length();
		if ((o1speed.Length() * 2 < ((gravity / m1) * coef * coef).Length()) && (o2speed.Length() < 0.0005))
		{
			momentum = momentum * (1 + pDepth * 30);
		}
		/*Vector3 test1 = normal * (momentum / m1);
		Vector3 test2 = normal * sumVelocity;
		Vector3 test3 = normal * sumVelocity * (-1 - damping);
		Vector3 test4 = normal *normal;*/
		//Setting speed of the objects appropriately
		speed1 = o1speed + (normal * (momentum / m1));
		speed2 = o2speed - (normal * (momentum / m2));
	}

	o1->SetVerletSpeed(speed1);
	o2->SetVerletSpeed(speed2);
}

//Applies gravity to all the objects and sets it within the container for new objects to use it as well
void PhysicsContainer::ApplyGravity(const Vector3& g)
{
	this->gravity = g;
}
