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

	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[4];
	{
		vertexData[0].x = 0.0f;
		vertexData[0].y = 0.0f;
		vertexData[0].z = 0.0f;

		vertexData[1].x = 0.5f;
		vertexData[1].y = 0.5f;
		vertexData[1].z = 0.0f;

		vertexData[2].x = 0.5f;
		vertexData[2].y = 0.0f;
		vertexData[2].z = 0.0f;

		vertexData[3].x = 0.0f;
		vertexData[3].y = 0.5f;
		vertexData[3].z = 0.0f;
	}
	uint16_t indexData_1[6] = { 0,1,2,0,3,1 };

	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData2[3];
	{
		vertexData2[0].x = 0.0f;
		vertexData2[0].y = 0.0f;
		vertexData2[0].z = 0.0f;

		vertexData2[1].x = 0.5f;
		vertexData2[1].y = 0.5f;
		vertexData2[1].z = 0.0f;

		vertexData2[2].x = 0.5f;
		vertexData2[2].y = 0.0f;
		vertexData2[2].z = 0.0f;
	}
	uint16_t indexData_2[3] = { 0,1,2 };

	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData3[4];
	{
		vertexData3[0].x = 0.0f;
		vertexData3[0].y = 0.0f;
		vertexData3[0].z = 0.0f;

		vertexData3[1].x = 0.5f;
		vertexData3[1].y = 0.5f;
		vertexData3[1].z = 0.0f;

		vertexData3[2].x = 0.5f;
		vertexData3[2].y = 0.0f;
		vertexData3[2].z = 0.0f;

		vertexData3[3].x = 0.0f;
		vertexData3[3].y = 0.5f;
		vertexData3[3].z = 0.0f;
	}
	uint16_t indexData_3[6] = { 0,1,2,0,3,1 };

	//init gameObjects
	{
		if (!(result = gameObject1.InitializeMeshEffect(6, indexData_1, 4, vertexData, "data/Shaders/Fragment/animatedColor.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = Graphics::cMesh::Load(meshReplace, 3, indexData_2, 3, vertexData2)))
		{
			EAE6320_ASSERTF(false, "Failed Initializing mesh");
			return result;
		}
	}
	{
		if (!(result = Graphics::cMesh::Load(meshMain, 6, indexData_3, 4, vertexData3)))
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
