#include <Engine/Collision/Collision.h>

std::vector<eae6320::Collision::cCollider*> colliderList;

void eae6320::Collision::CollisionCheckUpdate()
{
	//AABB collision check
}

void eae6320::Collision::AddCollider(cCollider* collider)
{
	colliderList.push_back(collider);
}

void eae6320::Collision::RemoveCollider(cCollider* collider)
{
	colliderList.erase(std::remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());
}
