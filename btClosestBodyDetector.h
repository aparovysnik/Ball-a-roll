#pragma once
#include "btBulletDynamicsCommon.h"

class btClosestBodyDetector : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	btClosestBodyDetector(btCollisionObject* me, btVector3* camOrigin, btVector3* destOrigin) : btCollisionWorld::ClosestConvexResultCallback(*camOrigin, *destOrigin)
		, m_me(me)
	{
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//caller already does the filter on the m_closestHitFraction
		btAssert(convexResult.m_hitFraction <= m_closestHitFraction);

		if (convexResult.m_hitCollisionObject == m_me)
			return btScalar(0);
		m_closestHitFraction = convexResult.m_hitFraction;
		m_hitCollisionObject = convexResult.m_hitCollisionObject;
		if (normalInWorldSpace)
		{
			m_hitNormalWorld = convexResult.m_hitNormalLocal;
		}
		else
		{
			///need to transform normal into worldspace
			m_hitNormalWorld = m_hitCollisionObject->getWorldTransform().getBasis()*convexResult.m_hitNormalLocal;
		}
		m_hitPointWorld = convexResult.m_hitPointLocal;
		return convexResult.m_hitFraction;
	}
protected:
	btCollisionObject* m_me;
};
