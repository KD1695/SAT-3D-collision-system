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
	/// Update for collider - udpates collider transform
	/// </summary>
	/// <param name="i_secondCountToIntegrate">frame time in seconds from update</param>
	void cCollider::Update(float i_secondCountToIntegrate)
	{
		transform = rigidBodyState->PredictFutureTransform(i_secondCountToIntegrate);
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
		if(_isColliding)
		{
			for(size_t i=0; i<currentlyCollidingColliders.size(); i++)
			{
				if(currentlyCollidingColliders[i] == other)
				{
					return;
				}
			}
			currentlyCollidingColliders.push_back(other);
			if(isCallbackSetEnter)
				callback_function_on_enter(this, other);
		}
		else
		{
			for(size_t i=0; i<currentlyCollidingColliders.size(); i++)
			{
				if(currentlyCollidingColliders[i] == other)
				{
					if(isCallbackSetExit)
						callback_function_on_exit(this, other);
					currentlyCollidingColliders.erase(currentlyCollidingColliders.begin()+(int)i);
					break;
				}
			}
		}
	}

	/// <summary>
	/// Set a callback on collision enter
	/// </summary>
	/// <param name="func">function with 2 collider pointers as params</param>
	void cCollider::SetOnCollisionEnterCallback(std::function<bool(cCollider* self, cCollider* other)> func)
	{
		callback_function_on_enter = std::move(func);
		isCallbackSetEnter = true;
	}

	/// <summary>
	/// Set a callback on collision exit
	/// </summary>
	/// <param name="func">function with 2 collider pointers as params</param>
	void cCollider::SetOnCollisionExitCallback(std::function<bool(cCollider* self, cCollider* other)> func)
	{
		callback_function_on_exit = std::move(func);
		isCallbackSetExit = true;
	}

	/// <summary>
	/// Getter for is colliding state of collider
	/// </summary>
	/// <returns>bool isCollding for collider</returns>
	bool cCollider::GetIsColliding() const
	{
		return (!currentlyCollidingColliders.empty());
	}

	/// <summary>
	/// Check if collider is active or not
	/// </summary>
	/// <returns>bool isActive</returns>
	bool cCollider::GetIsActive() const
	{
		return isActive;
	}

	/// <summary>
	/// Set collider active/inactive status
	/// </summary>
	/// <param name="_isActive">bool activeState</param>
	void cCollider::SetIsActive(bool _isActive)
	{
		isActive = _isActive;
	}

	Physics::sRigidBodyState* cCollider::GetRigidBodyReference()
	{
		return rigidBodyState;
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
	/// Get up direction from transform
	/// </summary>
	/// <returns>sVector4 up normal</returns>
	sVector4 cCollider::GetUpNormal() const
	{
		return sVector4(transform.GetUpDirection().x, transform.GetUpDirection().y, transform.GetUpDirection().z, 1);
	}

	/// <summary>
	/// Get right direction from transform
	/// </summary>
	/// <returns>sVector4 right normal</returns>
	sVector4 cCollider::GetRightNormal() const
	{
		return sVector4(transform.GetRightDirection().x, transform.GetRightDirection().y, transform.GetRightDirection().z, 1);
	}

	/// <summary>
	/// Get back direction from transform
	/// </summary>
	/// <returns>sVector4 back normal</returns>
	sVector4 cCollider::GetBackNormal() const
	{
		return sVector4(transform.GetBackDirection().x, transform.GetBackDirection().y, transform.GetBackDirection().z, 1);
	}

	/// <summary>
	/// Receive vertexData for a mesh that can represent current state of the collider
	/// </summary>
	/// <param name="o_vertexData">vertexData representing the collider</param>
	/// <returns>size_t vertexCount</returns>
	size_t cCollider::GetColliderMeshVertexData(eae6320::Graphics::VertexFormats::sVertex_mesh** o_vertexData)
	{
		CalculateVertices();
		Graphics::VertexFormats::sVertex_mesh vertexData[8];
		for(size_t i=0; i<8; i++)
		{
			vertexData[i].x = colliderVertices[i].x;
			vertexData[i].y = colliderVertices[i].y;
			vertexData[i].z = colliderVertices[i].z + 0.001f;
			vertexData[i].r = 0;
			vertexData[i].g = 255;
			vertexData[i].b = 0;
			vertexData[i].a = 122;
		}
		memcpy(*o_vertexData, vertexData, sizeof(Graphics::VertexFormats::sVertex_mesh)*8);
		return 8;
	}

	/// <summary>
	/// Receive indexData for a mesh that can represent current state of the collider
	/// </summary>
	/// <param name="o_indexData">indexData representing the collider</param>
	/// <returns>size_t indexCount</returns>
	size_t cCollider::GetColliderMeshIndexData(uint16_t** o_indexData)
	{
		*o_indexData = colliderIndexData;
		return 36;
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

		//Front face starting top left going TL->TR->BL->BR
		colliderVertices[0].x = -size.x / 2;
		colliderVertices[0].y = size.y / 2;
		colliderVertices[0].z = size.z / 2;

		colliderVertices[1].x = size.x / 2;
		colliderVertices[1].y = size.y / 2;
		colliderVertices[1].z = size.z / 2;

		colliderVertices[2].x = -size.x / 2;
		colliderVertices[2].y = -size.y / 2;
		colliderVertices[2].z = size.z / 2;

		colliderVertices[3].x = size.x / 2;
		colliderVertices[3].y = -size.y / 2;
		colliderVertices[3].z = size.z / 2;

		//Back face starting top left going TL->TR->BL->BR
		colliderVertices[4].x = -size.x / 2;
		colliderVertices[4].y = size.y / 2;
		colliderVertices[4].z = -size.z / 2;

		colliderVertices[5].x = size.x / 2;
		colliderVertices[5].y = size.y / 2;
		colliderVertices[5].z = -size.z / 2;

		colliderVertices[6].x = -size.x / 2;
		colliderVertices[6].y = -size.y / 2;
		colliderVertices[6].z = -size.z / 2;

		colliderVertices[7].x = size.x / 2;
		colliderVertices[7].y = -size.y / 2;
		colliderVertices[7].z = -size.z / 2;

		sVector4 updatedVertices[8];
		//Rotation
		const Math::sVector eulerAngles = rigidBodyState->orientation.GetEulerVector();
		for(size_t i = 0; i < 8; i++)
		{
			updatedVertices[i] = ((cMatrix4x4::CreateXRotation(eulerAngles.x)
									* cMatrix4x4::CreateYRotation(eulerAngles.y))
									* cMatrix4x4::CreateZRotation(eulerAngles.z))
									* sVector4(colliderVertices[i].x, colliderVertices[i].y, colliderVertices[i].z, 1);
		}

		//Translation
		for(size_t i = 0; i < 8; i++)
		{
			updatedVertices[i] = cMatrix4x4::CreateTransform(rigidBodyState->position.x, rigidBodyState->position.y, rigidBodyState->position.z) * updatedVertices[i];
			colliderVertices[i] = Math::sVector(updatedVertices[i].x, updatedVertices[i].y, updatedVertices[i].z);
		}
	}
}