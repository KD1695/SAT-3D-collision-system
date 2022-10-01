#pragma once
#include <Engine/Math/sVector.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320
{
	namespace Components
	{
		class Camera
		{
			Math::cMatrix_transformation localTransform;
			Math::cMatrix_transformation cameraToProjectedTransform;
			Physics::sRigidBodyState rigidbody;

		public:
			Camera();
			void Update(float i_secondCountToIntegrate);
			void SetPosition(Math::sVector position);
			void SetVelocity(Math::sVector velocity);
			Math::cMatrix_transformation GetLocalTransform();
			Math::cMatrix_transformation GetCameraToProjectedTransform();
		};
	}
}