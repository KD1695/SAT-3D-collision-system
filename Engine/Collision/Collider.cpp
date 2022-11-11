#include <Engine/Collision/Collider.h>
#include <Engine/Collision/Collision.h>
#include <corecrt_math.h>
#include <Engine/Logging/Logging.h>

#include <utility>

namespace eae6320::Collision
{

	/// <summary>
	/// Create collider with new rigidBody state, no attached gameObject
	/// </summary>
	/// <param name="size">Dimensions of collider - default dimension is 1,1,1</param>
	cCollider::cCollider(eae6320::Math::sVector _size)
	{
		size = _size;
		rigidBodyState = new eae6320::Physics::sRigidBodyState();
		CalculateVertices();
		AddCollider(this);
	}

	/// <summary>
	/// Create collider with existing rigidBody state reference
	/// </summary>
	/// <param name="size">dimensions of the collider</param>
	/// <param name="rigidBodyState">reference to rigidBody state</param>
	cCollider::cCollider(eae6320::Physics::sRigidBodyState* _rigidBodyState, eae6320::Math::sVector _size)
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
	void cCollider::SetSize(float x, float y, float z)
	{
		size.x = x;
		size.y = y;
		size.z = z;

		CalculateVertices();
	}

	/// <summary>
	/// Destructor, removes collider reference from system
	/// </summary>
	cCollider::~cCollider()
	{
		RemoveCollider(this);
		rigidBodyState = nullptr;
	}

	/// <summary>
	/// Collision system uses this to set colliding state of collider and invoke callback
	/// </summary>
	/// <param name="_isColliding">Is currently colliding</param>
	/// <param name="other">other collider in collision</param>
	void cCollider::SetIsColliding(bool _isColliding, cCollider* other)
	{
		if (isColliding == _isColliding)
			return;
		isColliding = _isColliding;
		if(isColliding)
		{
			if(isCallbackSetEnter)
				callback_function_on_enter(this, other);
		}
		else
		{
			if(isCallbackSetExit)
				callback_function_on_exit(this, other);
		}
	}

	void cCollider::SetOnCollisionEnterCallback(std::function<bool(cCollider* self, cCollider* other)> func)
	{
		callback_function_on_enter = std::move(func);
		isCallbackSetEnter = true;
	}

	void cCollider::SetOnCollisionExitCallback(std::function<bool(cCollider* self, cCollider* other)> func)
	{
		callback_function_on_exit = std::move(func);
		isCallbackSetExit = true;
	}

	/// <summary>
	/// Getter for is colliding state of collider
	/// </summary>
	/// <returns>bool isCollding for collider</returns>
	bool cCollider::GetIsColliding()
	{
		return isColliding;
	}

	/// <summary>
	/// Getter for collider vertices
	/// </summary>
	/// <returns>sVector pointer to list of 8 vertices</returns>
	eae6320::Math::sVector* cCollider::GetColliderVertices()
	{
		return colliderVertices;
	}

	/// <summary>
	/// Calculate world position of collider vertices based on size and position 
	/// </summary>
	void cCollider::CalculateVertices()
	{
		//Position offset
		float xOffset = rigidBodyState->position.x;
		float yOffset = rigidBodyState->position.y;
		float zOffset = rigidBodyState->position.z;

		//bottom 4 vertices going clockwise from front left
		colliderVertices[0].x = -size.x / 2 + xOffset;
		colliderVertices[0].y = -size.y / 2 + yOffset;
		colliderVertices[0].z = size.z / 2 + zOffset;

		colliderVertices[1].x = -size.x / 2 + xOffset;
		colliderVertices[1].y = -size.y / 2 + yOffset;
		colliderVertices[1].z = -size.z / 2 + zOffset;

		colliderVertices[2].x = size.x / 2 + xOffset;
		colliderVertices[2].y = -size.y / 2 + yOffset;
		colliderVertices[2].z = -size.z / 2 + zOffset;

		colliderVertices[3].x = size.x / 2 + xOffset;
		colliderVertices[3].y = -size.y / 2 + yOffset;
		colliderVertices[3].z = size.z / 2 + zOffset;

		//top 4 vertices going clockwise from front left
		colliderVertices[4].x = -size.x / 2 + xOffset;
		colliderVertices[4].y = size.y / 2 + yOffset;
		colliderVertices[4].z = size.z / 2 + zOffset;

		colliderVertices[5].x = -size.x / 2 + xOffset;
		colliderVertices[5].y = size.y / 2 + yOffset;
		colliderVertices[5].z = -size.z / 2 + zOffset;

		colliderVertices[6].x = size.x / 2 + xOffset;
		colliderVertices[6].y = size.y / 2 + yOffset;
		colliderVertices[6].z = -size.z / 2 + zOffset;

		colliderVertices[7].x = size.x / 2 + xOffset;
		colliderVertices[7].y = size.y / 2 + yOffset;
		colliderVertices[7].z = size.z / 2 + zOffset;

		//Rotation
		sVector4 rotatedVertices[8];
		const Math::sVector eulerAngles = rigidBodyState->orientation.GetEulerVector();
		for(size_t i = 0; i < 8; i++)
		{
			rotatedVertices[i] = (cMatrix4x4::CreateXRotation(eulerAngles.x)
									* cMatrix4x4::CreateYRotation(eulerAngles.y)
									* cMatrix4x4::CreateZRotation(eulerAngles.z))
									* sVector4(colliderVertices[i].x, colliderVertices[i].y, colliderVertices[i].z, 1);
			colliderVertices[i] = Math::sVector(rotatedVertices[i].x, rotatedVertices[i].y, rotatedVertices[i].z);
		}
	}
}