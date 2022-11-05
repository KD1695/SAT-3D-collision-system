#pragma once
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
			
			void CalculateVertices();

		public:
			cCollider(eae6320::Math::sVector size = eae6320::Math::sVector(1, 1, 1));
			cCollider(eae6320::Math::sVector size, eae6320::Physics::sRigidBodyState* rigidBodyState);
			void SetSize(float x, float y, float z);
		};
	}
}