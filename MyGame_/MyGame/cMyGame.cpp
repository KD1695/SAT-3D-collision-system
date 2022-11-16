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
	eae6320::Graphics::SetMeshEffectData(camera, gameObjects, 3);
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

	//player rotation
	if (UserInput::IsKeyPressed('Z'))
	{
		//move left
		gameObjects[0].GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(0,1,0);
		gameObjects[0].GetRigidBodyReference()->angularSpeed = 0.1f;
	}
	else if (UserInput::IsKeyPressed('X'))
	{
		//move right
		gameObjects[0].GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(0,0,1);
		gameObjects[0].GetRigidBodyReference()->angularSpeed = 0.1f;
	}
	else if (UserInput::IsKeyPressed('C'))
	{
		//move up
		gameObjects[0].GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(1,0,0);
		gameObjects[0].GetRigidBodyReference()->angularSpeed = 0.1f;
	}
	else
	{
		//gameObjects[0].SetVelocity(Math::sVector(0, 0, 0));
		gameObjects[0].GetRigidBodyReference()->angularSpeed = 0.0f;
	}

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

	
		//load collider mesh for visual
		{
			auto result = eae6320::Results::Success;
			Graphics::VertexFormats::sVertex_mesh* vertexData = static_cast<Graphics::VertexFormats::sVertex_mesh*>(calloc(8, sizeof(Graphics::VertexFormats::sVertex_mesh)));
			uint16_t* indexData = static_cast<uint16_t*>(calloc(36, sizeof(uint16_t))) ;
			size_t vertexCount = collider[0].GetColliderMeshVertexData(&vertexData);
			size_t indexCount = collider[0].GetColliderMeshIndexData(&indexData);
			if (!(result = Graphics::cMesh::Load(colliderMesh, indexCount, indexData, vertexCount, vertexData)))
			{
				EAE6320_ASSERTF(false, "Failed Initializing Mesh for collider");
				//return result;
			}
			gameObjects[1].SetMesh(colliderMesh);
		}
	//}

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
	gameObjects[2].Update(i_elapsedSecondCount_sinceLastUpdate);
	camera.Update(i_elapsedSecondCount_sinceLastUpdate);
	collider[0].Update(i_elapsedSecondCount_sinceLastUpdate);
	collider[1].Update(i_elapsedSecondCount_sinceLastUpdate);
}

eae6320::cResult eae6320::cMyGame::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Game...");
	auto result = eae6320::Results::Success;

	//init gameObjects
	{
		if (!(result = gameObjects[0].InitializeMeshEffect("data/Meshes/cube.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	collider[0] = eae6320::Collision::cCollider(gameObjects[0].GetRigidBodyReference(), Math::sVector(2,2,2));
	collider[0].SetOnCollisionEnterCallback(&OnCollisionEnter);
	collider[0].SetOnCollisionExitCallback(&OnCollisionExit);
	
	//load collider mesh for visual
	{
		Graphics::VertexFormats::sVertex_mesh* vertexData = static_cast<Graphics::VertexFormats::sVertex_mesh*>(calloc(8, sizeof(Graphics::VertexFormats::sVertex_mesh)));
		uint16_t* indexData = static_cast<uint16_t*>(calloc(36, sizeof(uint16_t))) ;
		size_t vertexCount = collider[0].GetColliderMeshVertexData(&vertexData);
		size_t indexCount = collider[0].GetColliderMeshIndexData(&indexData);
		if (!(result = Graphics::cMesh::Load(colliderMesh, indexCount, indexData, vertexCount, vertexData)))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	//set collider mesh to new gameObject
	{
		if (!(result = gameObjects[1].InitializeMeshEffect("data/Meshes/cube.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
		gameObjects[1].SetMesh(colliderMesh);
	}

	{
		if (!(result = gameObjects[2].InitializeMeshEffect("data/Meshes/cube.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject");
			return result;
		}
	}
	gameObjects[2].GetRigidBodyReference()->position = eae6320::Math::sVector(1.1f, 0.5, 0);
	collider[1] = eae6320::Collision::cCollider(gameObjects[2].GetRigidBodyReference());

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

bool eae6320::OnCollisionEnter(Collision::cCollider* self, Collision::cCollider* other)
{
	eae6320::Logging::OutputMessage("Collision Enter");
	return true;
}

bool eae6320::OnCollisionExit(Collision::cCollider* self, Collision::cCollider* other)
{
	eae6320::Logging::OutputMessage("Collision Exit");
	return true;
}
