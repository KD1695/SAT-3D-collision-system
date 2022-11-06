#include <Engine/Collision/Collision.h>
#include <Engine/Collision/Collider.h>

namespace eae6320::Collision
{
	std::vector<cCollider*> colliderList;

	void CollisionSystemUpdate()
	{
		//AABB collision check
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			for (size_t j = 0; j < colliderList.size(); j++)
			{
				if (i == j)
				{
					continue;
				}
				else
				{
					colliderList[i]->CalculateVertices();
					colliderList[j]->CalculateVertices();
					if (CollisionCheck(colliderList[i], colliderList[j]) && CollisionCheck(colliderList[j], colliderList[i]))
					{
						colliderList[i]->SetIsColliding(true);
						colliderList[j]->SetIsColliding(true);
					}
					else
					{
						colliderList[i]->SetIsColliding(false);
						colliderList[j]->SetIsColliding(false);
					}
				}
			}
		}
	}

	bool CollisionCheck(cCollider* colliderA, cCollider* colliderB)
	{
		float minXA = colliderA->GetColliderVertices()[0].x;
		float maxXA = colliderA->GetColliderVertices()[0].x;
		float minYA = colliderA->GetColliderVertices()[0].y;
		float maxYA = colliderA->GetColliderVertices()[0].y;
		float minZA = colliderA->GetColliderVertices()[0].z;
		float maxZA = colliderA->GetColliderVertices()[0].z;

		float minXB = colliderB->GetColliderVertices()[0].x;
		float maxXB = colliderB->GetColliderVertices()[0].x;
		float minYB = colliderB->GetColliderVertices()[0].y;
		float maxYB = colliderB->GetColliderVertices()[0].y;
		float minZB = colliderB->GetColliderVertices()[0].z;
		float maxZB = colliderB->GetColliderVertices()[0].z;

		for (int i = 0; i < 8; i++)
		{
			if (colliderA->GetColliderVertices()[i].x < minXA)
				minXA = colliderA->GetColliderVertices()[i].x;
			if (colliderA->GetColliderVertices()[i].x > maxXA)
				maxXA = colliderA->GetColliderVertices()[i].x;
			if (colliderA->GetColliderVertices()[i].y < minYA)
				minYA = colliderA->GetColliderVertices()[i].y;
			if (colliderA->GetColliderVertices()[i].y > maxYA)
				maxYA = colliderA->GetColliderVertices()[i].y;
			if (colliderA->GetColliderVertices()[i].z < minZA)
				minZA = colliderA->GetColliderVertices()[i].z;
			if (colliderA->GetColliderVertices()[i].z > maxZA)
				maxZA = colliderA->GetColliderVertices()[i].z;

			if (colliderB->GetColliderVertices()[i].x < minXB)
				minXB = colliderB->GetColliderVertices()[i].x;
			if (colliderB->GetColliderVertices()[i].x > maxXB)
				maxXB = colliderB->GetColliderVertices()[i].x;
			if (colliderB->GetColliderVertices()[i].y < minYB)
				minYB = colliderB->GetColliderVertices()[i].y;
			if (colliderB->GetColliderVertices()[i].y > maxYB)
				maxYB = colliderB->GetColliderVertices()[i].y;
			if (colliderB->GetColliderVertices()[i].z < minZB)
				minZB = colliderB->GetColliderVertices()[i].z;
			if (colliderB->GetColliderVertices()[i].z > maxZB)
				maxZB = colliderB->GetColliderVertices()[i].z;
		}

		return (
			minXA <= maxXB &&
			maxXA >= minXB &&
			minYA <= maxYB &&
			maxYA >= minYB &&
			minZA <= maxZB &&
			maxZA >= minZB
			);
	}

	void AddCollider(cCollider* collider)
	{
		colliderList.push_back(collider);
	}

	void RemoveCollider(cCollider* collider)
	{
		colliderList.erase(std::remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());
	}

}