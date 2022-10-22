#include <Engine/Components/Camera.h>

eae6320::Components::Camera::Camera()
{
	SetPosition(Math::sVector( 0, 0, 10));
	cameraToProjectedTransform = Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(0.8f, 1, 0.1f, 200.0f);
}

void eae6320::Components::Camera::Update(float i_secondCountToIntegrate)
{
	localTransform = rigidbody.PredictFutureTransform(i_secondCountToIntegrate);
	rigidbody.Update(i_secondCountToIntegrate);
}

void eae6320::Components::Camera::SetPosition(Math::sVector position)
{
	rigidbody.position = position;
}

void eae6320::Components::Camera::SetVelocity(Math::sVector velocity)
{
	rigidbody.velocity = velocity;
}

eae6320::Math::cMatrix_transformation eae6320::Components::Camera::GetLocalTransform()
{
	return localTransform;
}

eae6320::Math::cMatrix_transformation eae6320::Components::Camera::GetCameraToProjectedTransform()
{
	return cameraToProjectedTransform;
}