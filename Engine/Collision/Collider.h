#pragma once
#include <functional>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/sVector.h>
#include <Engine/Collision/Matrix.h>

#include "Engine/Graphics/Graphics.h"

namespace eae6320
{
	namespace Collision
	{
		/// <summary>
		/// Collider representation class. A 3d box collider.
		/// </summary>
		class cCollider
		{
		private:
			eae6320::Physics::sRigidBodyState* rigidBodyState = nullptr;
			eae6320::Math::sVector size = eae6320::Math::sVector(1, 1, 1);
			eae6320::Math::sVector colliderVertices[8];
			bool isColliding = false;
			bool isActive = true;
			std::function<bool(cCollider*, cCollider*)> callback_function_on_enter;
			std::function<bool(cCollider*, cCollider*)> callback_function_on_exit;
			bool isCallbackSetEnter = false;
			bool isCallbackSetExit = false;
			uint16_t colliderIndexData[36] = {
				2,3,0,
				2,7,3,
				3,7,1,
				0,3,1,
				0,1,4,
				1,7,5,
				4,2,0,
				4,1,5,
				4,5,6,
				6,5,7,
				6,7,2,
				6,2,4
			};
			eae6320::Math::cMatrix_transformation transform;

		public:
			void CalculateVertices();
			cCollider(eae6320::Math::sVector size = eae6320::Math::sVector(1, 1, 1));
			cCollider(eae6320::Physics::sRigidBodyState* rigidBodyState, eae6320::Math::sVector size = eae6320::Math::sVector(1, 1, 1));
			void SetSize(float x, float y, float z);
			void Update(float i_secondCountToIntegrate);
			void SetIsColliding(bool isColliding, cCollider* other);
			bool GetIsColliding() const;
			bool GetIsActive() const;
			void SetIsActive(bool _isActive);
			eae6320::Math::sVector* GetColliderVertices();
			sVector4 GetUpNormal() const;
			sVector4 GetRightNormal() const;
			sVector4 GetBackNormal() const;
			size_t GetColliderMeshVertexData(eae6320::Graphics::VertexFormats::sVertex_mesh** o_vertexData);
			size_t GetColliderMeshIndexData(uint16_t** o_indexData);
			void SetOnCollisionEnterCallback(std::function<bool(cCollider* self, cCollider* other)> func);
			void SetOnCollisionExitCallback(std::function<bool(cCollider* self, cCollider* other)> func);
			~cCollider();
		};
	}
}
