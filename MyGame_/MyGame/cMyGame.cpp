// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::SetBgColor(bg_Color);
	if (switchMeshFlag)
	{
		eae6320::Graphics::SetMeshEffectData(meshEffectPairs2, meshEffectPairCount-1);
	}
	else
		eae6320::Graphics::SetMeshEffectData(meshEffectPairs1, meshEffectPairCount);
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	if (UserInput::IsKeyPressed('Q'))
		switchMeshFlag = true;
	else
		switchMeshFlag = false;
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
	
	//load effects
	{
		if (!(result = eae6320::Graphics::cEffect::Load(meshEffectPairs1[0].effect, "data/Shaders/Fragment/animatedColor.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	{
		if (!(result = eae6320::Graphics::cEffect::Load(meshEffectPairs1[1].effect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	{
		if (!(result = eae6320::Graphics::cEffect::Load(meshEffectPairs2[0].effect, "data/Shaders/Fragment/newColor.shader")))
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
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData2[4];
	{
		vertexData2[0].x = 0.0f;
		vertexData2[0].y = 0.0f;
		vertexData2[0].z = 0.0f;

		vertexData2[1].x = -1.0f;
		vertexData2[1].y = -1.0f;
		vertexData2[1].z = 0.0f;

		vertexData2[2].x = -1.0f;
		vertexData2[2].y = 0.0f;
		vertexData2[2].z = 0.0f;

		vertexData2[3].x = 0.0f;
		vertexData2[3].y = -1.0f;
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
	uint16_t indexData_1[6] = { 0,1,2,0,3,1 };
	uint16_t indexData_2[6] = { 0,1,2,0,3,1 };
	uint16_t indexData_3[6] = { 0,1,2,0,3,1 };
	//load meshes
	{
		if (!(result = eae6320::Graphics::cMesh::Load(meshEffectPairs1[0].mesh, 6, indexData_1, 4, vertexData)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}
	{
		if (!(result = eae6320::Graphics::cMesh::Load(meshEffectPairs1[1].mesh, 6, indexData_2, 4, vertexData2)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}
	{
		if (!(result = eae6320::Graphics::cMesh::Load(meshEffectPairs2[0].mesh, 6, indexData_3, 4, vertexData3)))
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

	for (size_t i = 0; i < meshEffectPairCount; i++)
	{
		if (meshEffectPairs1[i].effect != nullptr)
		{
			meshEffectPairs1[i].effect->DecrementReferenceCount();
			meshEffectPairs1[i].effect = nullptr;
		}

		if (meshEffectPairs1[i].mesh != nullptr)
		{
			meshEffectPairs1[i].mesh->DecrementReferenceCount();
			meshEffectPairs1[i].mesh = nullptr;
		}
	}

	if (meshEffectPairs2[0].effect != nullptr)
	{
		meshEffectPairs2[0].effect->DecrementReferenceCount();
		meshEffectPairs2[0].effect = nullptr;
	}

	if (meshEffectPairs2[0].mesh != nullptr)
	{
		meshEffectPairs2[0].mesh->DecrementReferenceCount();
		meshEffectPairs2[0].mesh = nullptr;
	}

	return Results::Success;
}
