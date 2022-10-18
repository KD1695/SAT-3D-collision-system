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
	eae6320::Graphics::SetMeshEffectData(camera, &gameObject1, objectCount);
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	//player movement
	if (UserInput::IsKeyPressed('A'))
	{
		//move left
		gameObject1.SetVelocity(Math::sVector(-10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('D'))
	{
		//move right
		gameObject1.SetVelocity(Math::sVector(10, 0, 0));
	}
	else if (UserInput::IsKeyPressed('W'))
	{
		//move up
		gameObject1.SetVelocity(Math::sVector(0, 10, 0));
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		//move down
		gameObject1.SetVelocity(Math::sVector(0, -10, 0));
	}
	else
		gameObject1.SetVelocity(Math::sVector(0, 0, 0));

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
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		//change mesh
		if (!changeMeshFlag)
		{
			gameObject1.SetMesh(meshReplace);
			changeMeshFlag = true;
		}
		else
		{
			gameObject1.SetMesh(meshMain);
			changeMeshFlag = false;
		}
	}
}

// Initialize / Clean Up
//----------------------

void eae6320::cMyGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	gameObject1.Update(i_elapsedSecondCount_sinceLastUpdate);
	camera.Update(i_elapsedSecondCount_sinceLastUpdate);
}

eae6320::cResult eae6320::cMyGame::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Game...");
	auto result = eae6320::Results::Success;

	//init gameObjects
	{
		if (!(result = gameObject1.InitializeMeshEffect("data/Meshes/pyramid.json", "data/Shaders/Fragment/animatedColor.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = Graphics::cMesh::LoadFromFile(meshReplace, "data/Meshes/triangle.json")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing mesh");
			return result;
		}
	}
	{
		if (!(result = Graphics::cMesh::LoadFromFile(meshMain, "data/Meshes/pyramid.json")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing mesh");
			return result;
		}
	}

	return result;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Game...");

	gameObject1.CleanUp();
	gameObject2.CleanUp();

	return Results::Success;
}
