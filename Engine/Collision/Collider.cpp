#include <Engine/Collision/Collider.h>
#include <Engine/Collision/Collision.h>
#include <corecrt_math.h>

/// <summary>
/// Create collider with new rigidBody state, no attached gameObject
/// </summary>
/// <param name="size">Dimensions of collider - default dimension is 1,1,1</param>
eae6320::Collision::cCollider::cCollider(eae6320::Math::sVector _size)
{
	size = _size;
	*rigidBodyState = eae6320::Physics::sRigidBodyState();
	CalculateVertices();
	AddCollider(this);
}

/// <summary>
/// Create collider with existing rigidBody state reference
/// </summary>
/// <param name="size">dimensions of the collider</param>
/// <param name="rigidBodyState">reference to rigidBody state</param>
eae6320::Collision::cCollider::cCollider(eae6320::Math::sVector _size, eae6320::Physics::sRigidBodyState* _rigidBodyState)
{
	size = _size;
	rigidBodyState = _rigidBodyState;
	CalculateVertices();
	AddCollider(this);
}

/// <summary>
/// Set Collider size
/// </summary>
/// <param name="x">length(xAxis)</param>
/// <param name="y">height(yAxis)</param>
/// <param name="z">depth(zAxis)</param>
void eae6320::Collision::cCollider::SetSize(float x, float y, float z)
{
	size.x = x;
	size.y = y;
	size.z = z;

	CalculateVertices();
}

eae6320::Collision::cCollider::~cCollider()
{
	RemoveCollider(this);
}

/// <summary>
/// Calculate world position of collider vertices based on size and position 
/// </summary>
void eae6320::Collision::cCollider::CalculateVertices()
{
	//bottom 4 vertices going clockwise from front left
	colliderVertices[0].x = -size.x / 2;
	colliderVertices[0].y = -size.y / 2;
	colliderVertices[0].z = size.z / 2;

	colliderVertices[1].x = -size.x / 2;
	colliderVertices[1].y = -size.y / 2;
	colliderVertices[1].z = -size.z / 2;

	colliderVertices[2].x = size.x / 2;
	colliderVertices[2].y = -size.y / 2;
	colliderVertices[2].z = -size.z / 2;

	colliderVertices[3].x = size.x / 2;
	colliderVertices[3].y = -size.y / 2;
	colliderVertices[3].z = size.z / 2;

	//top 4 vertices going clockwise from front left
	colliderVertices[4].x = -size.x / 2;
	colliderVertices[4].y = size.y / 2;
	colliderVertices[4].z = size.z / 2;

	colliderVertices[5].x = -size.x / 2;
	colliderVertices[5].y = size.y / 2;
	colliderVertices[5].z = -size.z / 2;

	colliderVertices[6].x = size.x / 2;
	colliderVertices[6].y = size.y / 2;
	colliderVertices[6].z = -size.z / 2;

	colliderVertices[7].x = size.x / 2;
	colliderVertices[7].y = size.y / 2;
	colliderVertices[7].z = size.z / 2;
}