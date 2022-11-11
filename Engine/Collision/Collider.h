#pragma once
#include <functional>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/sVector.h>
#include <Engine/Collision/Matrix.h>

namespace eae6320
{
	namespace Collision
	{
		class cCollider
		{
		private:
			eae6320::Physics::sRigidBodyState* rigidBodyState = nullptr;
			eae6320::Math::sVector size = eae6320::Math::sVector(1, 1, 1);
			eae6320::Math::sVector colliderVertices[8];
			bool isColliding = false;
			std::function<bool(cCollider*, cCollider*)> callback_function_on_enter;
			std::function<bool(cCollider*, cCollider*)> callback_function_on_exit;
			bool isCallbackSetEnter = false;
			bool isCallbackSetExit = false;

		public:
			void CalculateVertices();
			cCollider(eae6320::Math::sVector size = eae6320::Math::sVector(1, 1, 1));
			cCollider(eae6320::Physics::sRigidBodyState* rigidBodyState, eae6320::Math::sVector size = eae6320::Math::sVector(1, 1, 1));
			void SetSize(float x, float y, float z);
			void SetIsColliding(bool isColliding, cCollider* other);
			bool GetIsColliding();
			eae6320::Math::sVector* GetColliderVertices();
			void SetOnCollisionEnterCallback(std::function<bool(cCollider* self, cCollider* other)> func);
			void SetOnCollisionExitCallback(std::function<bool(cCollider* self, cCollider* other)> func);
			~cCollider();
		};
	}
}