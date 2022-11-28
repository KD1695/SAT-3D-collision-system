// Includes
//=========

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>

#include "Engine/Graphics/Graphics.h"

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cFinalGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::SetBgColor(bg_Color);
	eae6320::Graphics::SetMeshEffectData(camera, gameObjects, objectCount);
}

void eae6320::cFinalGame::UpdateSimulationBasedOnInput()
{
	//player movement
	if (UserInput::IsKeyPressed('A'))
	{
		//move left
		camera.SetVelocity(Math::sVector(-10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('D'))
	{
		//move right
		camera.SetVelocity(Math::sVector(10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('W'))
	{
		//move up
		camera.SetVelocity(Math::sVector(0, 0, -10));
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		//move down
		camera.SetVelocity(Math::sVector(0, 0, 10));
	}
	else if (UserInput::IsKeyPressed('J'))
	{
		//move up
		camera.SetVelocity(Math::sVector(0, 10, 0));
	}
	else if (UserInput::IsKeyPressed('K'))
	{
		//move down
		camera.SetVelocity(Math::sVector(0, -10, 0));
	}
	else
		camera.SetVelocity(Math::sVector(0, 0, 0));

	//player rotation
	if (UserInput::IsKeyPressed('Z'))
	{
		//rotate Y
		camera.GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(0,1,0);
		camera.GetRigidBodyReference()->angularSpeed = 0.1f;
	}
	else if (UserInput::IsKeyPressed('X'))
	{
		//rotate Z
		camera.GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(0,0,1);
		camera.GetRigidBodyReference()->angularSpeed = 0.1f;
	}
	else if (UserInput::IsKeyPressed('C'))
	{
		//rotate X
		camera.GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(1,0,0);
		camera.GetRigidBodyReference()->angularSpeed = 0.1f;
	}
	else
	{
		camera.GetRigidBodyReference()->angularSpeed = 0.0f;
	}
}

void eae6320::cFinalGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cFinalGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	Physics::sRigidBodyState* cameraRigidBody = camera.GetRigidBodyReference();
	gameObjects[0].GetRigidBodyReference()->position = Math::sVector(cameraRigidBody->position.x, cameraRigidBody->position.y - 5, cameraRigidBody->position.z - 15);
	gameObjects[0].Update(i_elapsedSecondCount_sinceLastUpdate);
	gameObjects[1].Update(i_elapsedSecondCount_sinceLastUpdate);
	camera.Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Game...");
	auto result = eae6320::Results::Success;

	//init gameObjects
	{
		if (!(result = gameObjects[0].InitializeMeshEffect("data/Meshes/cube.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject")
			return result;
		}
	}

	{
		if (!(result = gameObjects[1].InitializeMeshEffect("data/Meshes/plane.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject")
			return result;
		}
	}
	return result;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Game...");

	gameObjects[0].CleanUp();
	gameObjects[1].CleanUp();
	
	return Results::Success;
}
