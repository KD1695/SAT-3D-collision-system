// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.cpp>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::SetBgColor(bg_Color);
	eae6320::Graphics::SetMeshEffectData(camera, gameObjects, objectCount);
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	//player movement
	if (UserInput::IsKeyPressed('A'))
	{
		//move left
		gameObjects[0].SetVelocity(Math::sVector(-10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('D'))
	{
		//move right
		gameObjects[0].SetVelocity(Math::sVector(10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('W'))
	{
		//move up
		gameObjects[0].SetVelocity(Math::sVector(0, 10, 0));
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		//move down
		gameObjects[0].SetVelocity(Math::sVector(0, -10, 0));
	}
	else
		gameObjects[0].SetVelocity(Math::sVector(0, 0, 0));

	//camera movement
	if (UserInput::IsKeyPressed('J'))
	{
		//move left
		camera.SetVelocity(Math::sVector(-10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('L'))
	{
		//move right
		camera.SetVelocity(Math::sVector(10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('I'))
	{
		//move up
		camera.SetVelocity(Math::sVector(0, 0, 10));
	}
	else if (UserInput::IsKeyPressed('K'))
	{
		//move down
		camera.SetVelocity(Math::sVector(0, 0, -10));
	}
	else
		camera.SetVelocity(Math::sVector(0, 0, 0));
}

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

// Initialize / Clean Up
//----------------------

void eae6320::cMyGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	gameObjects[0].Update(i_elapsedSecondCount_sinceLastUpdate);
	camera.Update(i_elapsedSecondCount_sinceLastUpdate);
}

eae6320::cResult eae6320::cMyGame::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Game...");
	auto result = eae6320::Results::Success;

	//init gameObjects
	{
		if (!(result = gameObjects[0].InitializeMeshEffect("data/Meshes/pyramid.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = gameObjects[1].InitializeMeshEffect("data/Meshes/torus.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = gameObjects[2].InitializeMeshEffect("data/Meshes/helix.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = gameObjects[3].InitializeMeshEffect("data/Meshes/plane.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}

	return result;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Game...");

	for (int i = 0; i < 4; i++)
	{
		gameObjects[i].CleanUp();
	}

	return Results::Success;
}
