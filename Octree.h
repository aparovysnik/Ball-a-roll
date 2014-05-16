#pragma once
#ifndef Octree_H
#define Octree_H

#include <cstddef>
#include <vector>
#include "OctreeNode.h"
#include "RenderObject.h"

	/**!
	*
	*/
	class Octree {
		// Physical position/size. This implicitly defines the bounding 
		// box of this node
		Vector3 origin;         //! The physical center of this node
		Vector3 halfDimension;  //! Half the width/height/depth of this node

		// The tree has up to eight children and can additionally store
		// a point, though in many applications only, the leaves will store data.
		Octree *children[8]; //! Pointers to child octants
		OctreeNode *data;   //! Data point to be stored at a node

		/*
		Children follow a predictable pattern to make accesses simple.
		Here, - means less than 'origin' in that dimension, + means greater than.
		child:	0 1 2 3 4 5 6 7
		x:      - - - - + + + +
		y:      - - + + - - + +
		z:      - + - + - + - +
		*/

	public:
		Octree(const Vector3& origin, const Vector3& halfDimension)
			: origin(origin), halfDimension(halfDimension), data(NULL) {
			// Initially, there are no children
			for (int i = 0; i<8; ++i)
				children[i] = NULL;
		}

		Octree(const Octree& copy)
			: origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {

		}

		~Octree() {
			// Recursively destroy octants
			for (int i = 0; i<8; ++i)
				delete children[i];
		}

		// Determine which octant of the tree would contain 'object'
		vector<int> GetChildrenContainingObject(GameEntity* ent) const {
			RenderObject* obj = ent->GetVisuals();
			Vector3 halfBoundingSize = obj->GetModelMatrix().GetScalingVector();
			Vector3 position = obj->GetModelMatrix().GetPositionVector();
			Vector3 childHalfDim = halfDimension * 0.5;
			vector<int> results;
			if (!IsLeafNode())
				for (int i = 0; i < 8; i++)
				{
					if ((children[i]->data != NULL) && (children[i]->data->containsObject(ent)))
					{
						results.push_back(i);
					}
				}
			return results;
		}

		void GetPotentialCollisions(GameEntity* ent, vector<GameEntity*>& results) const {
			RenderObject* obj = ent->GetVisuals();
			Vector3 halfBoundingSize = obj->GetModelMatrix().GetScalingVector();
			Vector3 position = obj->GetModelMatrix().GetPositionVector();
			Vector3 childHalfDim = halfDimension * 0.5;

			if (!IsLeafNode())
			for (int i = 0; i < 8; i++)
			{
				children[i]->GetPotentialCollisions(ent, results);
			}
			else if (data != NULL && data->containsObject(ent))
			{
				for each (GameEntity* res in data->getObjects())
				{
					if (res->GetVisuals() != obj && find(results.begin(), results.end(),res) == results.end()) 
					{
						results.push_back(res);
					}
				}
			}
		}

		void Remove(GameEntity* ent)
		{
			if (ent->GetPhysics() != NULL)
			{
				RenderObject* obj = ent->GetVisuals();
				Vector3 halfBoundingSize = obj->GetModelMatrix().GetScalingVector();
				Vector3 position = obj->GetModelMatrix().GetPositionVector();
				Vector3 childHalfDim = halfDimension * 0.5;

				if (!IsLeafNode())
				for (int i = 0; i < 8; i++)
				{
					children[i]->Remove(ent);
				}
				else
				{
					if (data != NULL)
						data->removeObject(ent);
				}
			}
		}

		bool IsLeafNode() const {
			// This is correct, but overkill. See below.
			/*
			for(int i=0; i<8; ++i)
			if(children[i] != NULL)
			return false;
			return true;
			*/

			// We are a leaf iff we have no children. Since we either have none, or 
			// all eight, it is sufficient to just check the first.
			return children[0] == NULL;
		}

		void Insert(GameEntity* ent) {
			if (ent->GetPhysics() != NULL)
			{
				RenderObject* obj = ent->GetVisuals();
				// If this node doesn't have a data point yet assigned 
				// and it is a leaf, then we're done!
				if (IsLeafNode()) {
					if (data == NULL) {
						data = new OctreeNode();
						data->addObject(ent);
						return;
					}
					else if (data->getObjects().size() < 8)
					{
						data->addObject(ent);
					}
					else {
						// We're at a leaf, but there's already enough objects here
						// We will split this node so that it has 8 child octants
						// and then insert the old data that was here, along with 
						// this new data point

						// Save this data point that was here for a later re-insert
						OctreeNode *oldPoint = data;
						data = NULL;



						// Split the current node and create new empty trees for each
						// child octant.
						for (int i = 0; i < 8; ++i) {
							// Compute new bounding box for this child
							Vector3 newOrigin = origin;
							newOrigin.x += halfDimension.x * (i & 4 ? .5f : -.5f);
							newOrigin.y += halfDimension.y * (i & 2 ? .5f : -.5f);
							newOrigin.z += halfDimension.z * (i & 1 ? .5f : -.5f);
							children[i] = new Octree(newOrigin, halfDimension*.5f);
						}

						// Re-insert the old point, and insert this new point
						// (We wouldn't need to insert from the root, because we already
						// know it's guaranteed to be in this section of the tree)

						vector<GameEntity*> oldEntities = oldPoint->getObjects();
						for each (GameEntity* oldEnt in oldEntities)
						{
							BoundingBox bBox = oldEnt->GetPhysics()->GetBoundingBox();
							vector<Octree*> contains;
							GetOctantsInsideBox(bBox.backBottomLeft, bBox.frontTopRight, contains);
							for each (Octree* oc in contains)
							{
								oc->Insert(oldEnt);
							}
						}

						BoundingBox bBox = ent->GetPhysics()->GetBoundingBox();
						vector<Octree*> contains;
						GetOctantsInsideBox(bBox.backBottomLeft, bBox.frontTopRight, contains);
						for each (Octree* oc in contains)
						{
							oc->Insert(ent);
						}
					}
				}
				else {
					// We are at an interior node. Insert recursively into the 
					// appropriate child octant

					BoundingBox bBox = ent->GetPhysics()->GetBoundingBox();
					vector<Octree*> contains;
					GetChildrenInsideBox(bBox.backBottomLeft, bBox.frontTopRight, contains);
					for each (Octree* octant in contains)
					{
						octant->Insert(ent);
					}
				}
			}
		}

		void Update(GameEntity* ent)
		{
			Remove(ent);
			Insert(ent);
		}

		// This is a really simple routine for querying the tree for points
		// within a bounding box defined by min/max points (bmin, bmax)
		// All results are pushed into 'results'
		void GetOctantsInsideBox(const Vector3& bmin, const Vector3& bmax, std::vector<OctreeNode*>& results) {
			// If we're at a leaf node, just see if the current data point is inside
			// the query bounding box
			if (IsLeafNode()) {
				if (data != NULL) {
					vector<GameEntity*> entities = data->getObjects();
					for each (GameEntity* ent in entities)
					{
						RenderObject* obj = ent->GetVisuals();
						Vector3 frontBottomLeft = obj->GetModelMatrix().GetPositionVector() - obj->GetModelMatrix().GetScalingVector();
						Vector3 backTopRight = obj->GetModelMatrix().GetPositionVector() + obj->GetModelMatrix().GetScalingVector();
						if (backTopRight.x>bmax.x || backTopRight.y>bmax.y || backTopRight.z>bmax.z) return;
						if (frontBottomLeft.x<bmin.x || frontBottomLeft.y<bmin.y || frontBottomLeft.z<bmin.z) return;
						results.push_back(data);
					}
				}
			}
			else {
				// We're at an interior node of the tree. We will check to see if
				// the query bounding box lies outside the octants of this node.
				for (int i = 0; i<8; ++i) {
					// Compute the min/max corners of this child octant
					Vector3 cmax = children[i]->origin + children[i]->halfDimension;
					Vector3 cmin = children[i]->origin - children[i]->halfDimension;

					// If the query rectangle is outside the child's bounding box, 
					// then continue
					if (cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
					if (cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

					// At this point, we've determined that this child is intersecting 
					// the query bounding box
					children[i]->GetOctantsInsideBox(bmin, bmax, results);
				}
			}
		}

		void GetOctantsInsideBox(const Vector3& bmin, const Vector3& bmax, std::vector<Octree*>& results) {
			// If we're at a leaf node, just see if the current data point is inside
			// the query bounding box
			if (IsLeafNode()) {
				results.push_back(this);
			}
			else {
				// We're at an interior node of the tree. We will check to see if
				// the query bounding box lies outside the octants of this node.
				for (int i = 0; i<8; ++i) {
					// Compute the min/max corners of this child octant
					Vector3 cmax = children[i]->origin + children[i]->halfDimension;
					Vector3 cmin = children[i]->origin - children[i]->halfDimension;

					// If the query rectangle is outside the child's bounding box, 
					// then continue
					if (cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
					if (cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

					// At this point, we've determined that this child is intersecting 
					// the query bounding box
					children[i]->GetOctantsInsideBox(bmin, bmax, results);
				}
			}
		}

		void GetChildrenInsideBox(const Vector3& bmin, const Vector3& bmax, std::vector<Octree*>& results) {
				// We're at an interior node of the tree. We will check to see if
				// the query bounding box lies outside the octants of this node.
				for (int i = 0; i<8; ++i) {
					// Compute the min/max corners of this child octant
					Vector3 cmax = children[i]->origin + children[i]->halfDimension;
					Vector3 cmin = children[i]->origin - children[i]->halfDimension;

					// If the query rectangle is outside the child's bounding box, 
					// then continue
					if (cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
					if (cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

					// At this point, we've determined that this child is intersecting 
					// the query bounding box
					results.push_back(children[i]);
				}
		}
	};



#endif

