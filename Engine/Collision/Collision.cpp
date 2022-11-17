#include <Engine/Collision/Collision.h>

namespace eae6320::Collision
{
	std::vector<cCollider*> colliderList;

	/// <summary>
	/// Main function that checks for collisions between all colliders active in system and invokes relevant callbacks. Called in iApplication update.
	/// </summary>
	void CollisionSystemUpdate()
	{
		//AABB collision check
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			if(!colliderList[i]->GetIsActive())
			{
				continue;
			}
			for (size_t j = 0; j < colliderList.size(); j++)
			{
				if (i == j || !colliderList[j]->GetIsActive())
				{
					continue;
				}
				else
				{
					colliderList[i]->CalculateVertices();
					colliderList[j]->CalculateVertices();

					sVector4 listOfAxisToCheck[15];
					//face normals
					listOfAxisToCheck[0] = colliderList[i]->GetUpNormal();
					listOfAxisToCheck[1] = colliderList[i]->GetRightNormal();
					listOfAxisToCheck[2] = colliderList[i]->GetBackNormal();
					listOfAxisToCheck[3] = colliderList[j]->GetUpNormal();
					listOfAxisToCheck[4] = colliderList[j]->GetRightNormal();
					listOfAxisToCheck[5] = colliderList[j]->GetBackNormal();
					//normals cross product 
					listOfAxisToCheck[6] = listOfAxisToCheck[0] * listOfAxisToCheck[3];
					listOfAxisToCheck[7] = listOfAxisToCheck[0] * listOfAxisToCheck[4];
					listOfAxisToCheck[8] = listOfAxisToCheck[0] * listOfAxisToCheck[5];
					listOfAxisToCheck[9] = listOfAxisToCheck[1] * listOfAxisToCheck[3];
					listOfAxisToCheck[10] = listOfAxisToCheck[1] * listOfAxisToCheck[4];
					listOfAxisToCheck[11] = listOfAxisToCheck[1] * listOfAxisToCheck[5];
					listOfAxisToCheck[12] = listOfAxisToCheck[2] * listOfAxisToCheck[3];
					listOfAxisToCheck[13] = listOfAxisToCheck[2] * listOfAxisToCheck[4];
					listOfAxisToCheck[14] = listOfAxisToCheck[2] * listOfAxisToCheck[5];

					for(size_t k=0; k<15; k++)
					{
						if(!CollisionCheckSAT(colliderList[i], colliderList[j], listOfAxisToCheck[k]))
						{
							colliderList[i]->SetIsColliding(false, colliderList[j]);
							colliderList[j]->SetIsColliding(false, colliderList[i]);
							break;
						}
						else if(k==14)
						{
							colliderList[i]->SetIsColliding(true, colliderList[j]);
							colliderList[j]->SetIsColliding(true, colliderList[i]);
						}
					}

					// if (CollisionCheckAABB(colliderList[i], colliderList[j]) && CollisionCheckAABB(colliderList[j], colliderList[i]))
					// {
					// 	colliderList[i]->SetIsColliding(true, colliderList[j]);
					// 	colliderList[j]->SetIsColliding(true, colliderList[i]);
					// }
					// else
					// {
					// 	colliderList[i]->SetIsColliding(false, colliderList[j]);
					// 	colliderList[j]->SetIsColliding(false, colliderList[i]);
					// }
				}
			}
		}
	}

	/// <summary>
	/// AABB Collision/intersection check between two axis aligned colliders
	/// </summary>
	/// <param name="colliderA">collider reference A</param>
	/// <param name="colliderB">collider reference B</param>
	/// <returns>bool true if colliders intersect</returns>
	bool CollisionCheckAABB(cCollider* colliderA, cCollider* colliderB)
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

	inline float Min(float a, float b)
	{
		return (a < b) ? a : b;
	}

	inline float Max(float a, float b)
	{
		return (a > b) ? a : b;
	}
	
	/// <summary>
	/// Separating Axis Theorem based collision detection check given 2 colliders and 1 axis
	/// </summary>
	/// <param name="colliderA">colliderA</param>
	/// <param name="colliderB">colliderB</param>
	/// <param name="axis">Axis of separation to check for</param>
	/// <returns>bool isColliding</returns>
	bool CollisionCheckSAT(cCollider* colliderA, cCollider* colliderB, sVector4 axis)
	{
		if(axis.x == 0 && axis.y == 0 && axis.z == 0)
			return true;
		float maxProjectionA = FLT_MIN;
		float minProjectionA = FLT_MAX;
		for(size_t i=0; i<8; i++)
		{
			sVector4 vertexWorldPositon = sVector4(colliderA->GetColliderVertices()[i].x, colliderA->GetColliderVertices()[i].y, colliderA->GetColliderVertices()[i].z, 1);
			float projection = vertexWorldPositon.x * axis.x + vertexWorldPositon.y * axis.y + vertexWorldPositon.z * axis.z;
			maxProjectionA = Max(maxProjectionA, projection);
			minProjectionA = Min(minProjectionA, projection);
		}

		float maxProjectionB = FLT_MIN;
		float minProjectionB = FLT_MAX;
		for(size_t i=0; i<8; i++)
		{
			sVector4 vertexWorldPositon = sVector4(colliderB->GetColliderVertices()[i].x, colliderB->GetColliderVertices()[i].y, colliderB->GetColliderVertices()[i].z, 1);
			float projection = vertexWorldPositon.x * axis.x + vertexWorldPositon.y * axis.y + vertexWorldPositon.z * axis.z;
			maxProjectionB = Max(maxProjectionB, projection);
			minProjectionB = Min(minProjectionB, projection);
		}

		//compare overlap
		if ((minProjectionA <= maxProjectionB && minProjectionA >= minProjectionB) || (minProjectionB <= maxProjectionA && minProjectionB >= minProjectionA))
		{
			//overlap exists
			return true;
		}
		else
		{
			return false;
		}
	}

	/// <summary>
	/// Add reference of param collider to collider list
	/// </summary>
	/// <param name="collider">collider reference to add</param>
	void AddCollider(cCollider* collider)
	{
		colliderList.push_back(collider);
	}

	/// <summary>
	/// Removes given reference from collider list
	/// </summary>
	/// <param name="collider">collider reference to remove</param>
	void RemoveCollider(cCollider* collider)
	{
		colliderList.erase(std::remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());
	}

}