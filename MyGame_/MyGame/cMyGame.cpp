// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::SetBgColor(bg_Color);
	eae6320::Graphics::SetEffectData(effect);
	eae6320::Graphics::SetMeshData(mesh);
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

eae6320::cResult eae6320::cMyGame::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Game...");
	auto result = eae6320::Results::Success;
	{
		if (!(result = eae6320::Graphics::cEffect::Load(effect, "data/Shaders/Fragment/animatedColor.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[4];
	{
		vertexData[0].x = 0.0f;
		vertexData[0].y = 0.0f;
		vertexData[0].z = 0.0f;

		vertexData[1].x = 1.0f;
		vertexData[1].y = 1.0f;
		vertexData[1].z = 0.0f;

		vertexData[2].x = 1.0f;
		vertexData[2].y = 0.0f;
		vertexData[2].z = 0.0f;

		vertexData[3].x = 0.0f;
		vertexData[3].y = 1.0f;
		vertexData[3].z = 0.0f;
	}
	uint16_t indexData_1[6] = { 0,1,2,0,3,1 };
	{
		if (!(result = eae6320::Graphics::cMesh::Load(mesh, 6, indexData_1, 4, vertexData)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}
	return result;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Game...");

	if (mesh != nullptr)
	{
		mesh->DecrementReferenceCount();
		mesh = nullptr;
	}
	if (effect != nullptr)
	{
		effect->DecrementReferenceCount();
		effect = nullptr;
	}

	return Results::Success;
}
