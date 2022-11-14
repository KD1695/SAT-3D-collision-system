#pragma once
#include <vector>

#include "Collider.h"

namespace eae6320
{
	namespace Collision
	{
		class cCollider;
		
		void AddCollider(cCollider* collider);
		void RemoveCollider(cCollider* collider);
		void CollisionSystemUpdate();
		bool CollisionCheckAABB(cCollider* colliderA, cCollider* colliderB);
		bool CollisionCheckSAT(cCollider* colliderA, cCollider* colliderB, sVector4 axis);
	}
}
