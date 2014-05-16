#pragma once
#include "../nclgl/Vector3.h"
#include "GameEntity.h"
#include <vector>
class OctreeNode
{
	vector<GameEntity*> objects;
	public:
		OctreeNode() { }
		//OctreeNode(const vector<RenderObject*>& position) : position(position) { }
		inline void addObject(GameEntity* obj) { objects.push_back(obj); }
		inline vector<GameEntity*>& getObjects() { return objects; }
		inline bool containsObject(GameEntity* obj) { return ((objects.size() > 0) && (std::find(objects.begin(), objects.end(), obj) != objects.end())); }
		inline void removeObject(GameEntity* obj)
		{
			vector<GameEntity*>::iterator iter = std::find(objects.begin(), objects.end(), obj);
			if (iter != objects.end())
			{
				objects.erase(iter);
			}
		}
};


