#pragma once
#include <vector>

namespace eae6320
{
	namespace Collision
	{
		class cCollider;

		void AddCollider(cCollider* collider);
		void RemoveCollider(cCollider* collider);
		void CollisionCheckUpdate();
	}
}