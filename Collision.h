#pragma once
#ifndef COLLISION_H
#define COLLISION_H
#include <map>
#include "../nclgl/Vector3.h"
namespace Game
{
	struct CollisionSettings
	{
		CollisionSettings() {};
		CollisionSettings(Vector3 n, float d)
		{
			this->normal = n;
			this->distance = d;
		}
		Vector3 normal;
		float distance;
	};
}
#endif
