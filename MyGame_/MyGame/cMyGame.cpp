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
	eae6320::Graphics::SetMeshEffectData(gameObjects1, objectCount-1);
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	if (UserInput::IsKeyPressed('A'))
	{
		//move left
		gameObjects1[0].SetVelocity(Math::sVector(-1, 0, 0));
	}
	else if (UserInput::IsKeyPressed('D'))
	{
		//move right
		gameObjects1[0].SetVelocity(Math::sVector(1, 0, 0));
	}
	else if (UserInput::IsKeyPressed('W'))
	{
		//move up
		gameObjects1[0].SetVelocity(Math::sVector(0, 1, 0));
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		//move down
		gameObjects1[0].SetVelocity(Math::sVector(0, -1, 0));
	}
	else
		gameObjects1[0].SetVelocity(Math::sVector(0, 0, 0));
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
	gameObjects1[0].Update(i_elapsedSecondCount_sinceLastUpdate);
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
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData2[4];
	{
		vertexData2[0].x = 0.0f;
		vertexData2[0].y = 0.0f;
		vertexData2[0].z = 0.0f;

		vertexData2[1].x = -0.5f;
		vertexData2[1].y = -0.5f;
		vertexData2[1].z = 0.0f;

		vertexData2[2].x = -0.5f;
		vertexData2[2].y = 0.0f;
		vertexData2[2].z = 0.0f;

		vertexData2[3].x = 0.0f;
		vertexData2[3].y = -0.5f;
		vertexData2[3].z = 0.0f;
	}
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData3[4];
	{
		vertexData3[0].x = 0.0f;
		vertexData3[0].y = 0.0f;
		vertexData3[0].z = 0.0f;

		vertexData3[1].x = -1.0f;
		vertexData3[1].y = -1.0f;
		vertexData3[1].z = 0.0f;

		vertexData3[2].x = -1.0f;
		vertexData3[2].y = 0.0f;
		vertexData3[2].z = 0.0f;

		vertexData3[3].x = 0.0f;
		vertexData3[3].y = -1.0f;
		vertexData3[3].z = 0.0f;
	}
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData4[4];
	{
		vertexData4[0].x = 0.0f;
		vertexData4[0].y = 0.0f;
		vertexData4[0].z = 0.0f;

		vertexData4[1].x = 1.0f;
		vertexData4[1].y = 1.0f;
		vertexData4[1].z = 0.0f;

		vertexData4[2].x = 1.0f;
		vertexData4[2].y = 0.0f;
		vertexData4[2].z = 0.0f;

		vertexData4[3].x = 0.0f;
		vertexData4[3].y = 1.0f;
		vertexData4[3].z = 0.0f;
	}
	uint16_t indexData_1[6] = { 0,1,2,0,3,1 };
	uint16_t indexData_2[6] = { 0,1,2,0,3,1 };
	uint16_t indexData_3[6] = { 0,1,2,0,3,1 };
	uint16_t indexData_4[6] = { 0,1,2,0,3,1 };

	//init gameObjects
	{
		if (!(result = gameObjects1[0].InitializeMeshEffect(6, indexData_1, 4, vertexData, "data/Shaders/Fragment/newColor.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = gameObjects1[1].InitializeMeshEffect(6, indexData_2, 4, vertexData2)))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = gameObjects2[0].InitializeMeshEffect(6, indexData_3, 4, vertexData3, "data/Shaders/Fragment/newColor.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	{
		if (!(result = gameObjects2[1].InitializeMeshEffect(6, indexData_4, 4, vertexData4)))
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

	for (size_t i = 0; i < objectCount; i++)
	{
		gameObjects1[i].CleanUp();
	}

	for (size_t i = 0; i < objectCount; i++)
	{
		gameObjects2[i].CleanUp();
	}

	return Results::Success;
}
