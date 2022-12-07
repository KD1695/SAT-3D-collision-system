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
	//get all gameObjects
	int count = 0;
	for(size_t i=0; i<levelBlocksCount; i++)
	{
		count += level_blocks[i].blockCount+1;
	}
	count++; //player
	allGameObjects = new Components::GameObject[count];

	int index = 0;
	for(size_t i=0; i<levelBlocksCount; i++)
	{
		level_blocks[i].GetGameObjects(allGameObjects, index);
		index += level_blocks->blockCount+1;
	}
	allGameObjects[count-1] = ship;
	
	eae6320::Graphics::SetBgColor(bg_Color);
	eae6320::Graphics::SetMeshEffectData(camera, allGameObjects, count);
}

void eae6320::cFinalGame::UpdateSimulationBasedOnInput()
{
	//player movement
	if (UserInput::IsKeyPressed('A') && !isLeftKeyPressed)
	{
		//move left
		isTargetSet = true;
		isLeftKeyPressed = true;
		currentTargetXPos -= (currentTargetXPos-movementDistance >= -(movementDistance*lanesOnEachSide)) ? movementDistance : 0;
	}
	else if (UserInput::IsKeyPressed('D') && !isRightKeyPressed)
	{
		//move right
		isTargetSet = true;
		isRightKeyPressed = true;
		currentTargetXPos += (currentTargetXPos+movementDistance <= (movementDistance*lanesOnEachSide)) ? movementDistance : 0;
	}
	else if (UserInput::IsKeyPressed('W') && !isStopped)
	{
		//move up
		camera.SetVelocity(Math::sVector(0, 0, -shipSpeed));
	}
	else if (UserInput::IsKeyPressed('S') && !isStopped)
	{
		//move down
		camera.SetVelocity(Math::sVector(0, 0, shipSpeed));
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		//Jump
		JumpStart();
	}
	else
	{
		isLeftKeyPressed = false;
		isRightKeyPressed = false;
		camera.GetRigidBodyReference()->acceleration = Math::sVector(0,0,0);
		camera.SetVelocity(Math::sVector(0, 0, 0));
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

	//move player if target is set
	if(isTargetSet)
	{
		if(abs(cameraRigidBody->position.x - currentTargetXPos) < 0.1f)
		{
			isTargetSet = false;
			camera.SetVelocity(Math::sVector(0, 0, 0));
		}
		else if(cameraRigidBody->position.x > currentTargetXPos)
		{
			camera.SetVelocity(Math::sVector(-100, 0, 0));
		}
		else
		{
			camera.SetVelocity(Math::sVector(100, 0, 0));
		}
	}

	JumpUpdate(i_elapsedSecondCount_sinceLastUpdate);
	ship.GetRigidBodyReference()->position = Math::sVector(cameraRigidBody->position.x, cameraRigidBody->position.y - 5, cameraRigidBody->position.z - 15);
	ship.Update(i_elapsedSecondCount_sinceLastUpdate);
	camera.Update(i_elapsedSecondCount_sinceLastUpdate);
	for(size_t i=0; i<levelBlocksCount; i++)
	{
		if(level_blocks[i].plane.GetRigidBodyReference()->position.z - ship.GetRigidBodyReference()->position.z > 80)
		{
			level_blocks[i].UpdatePosition(level_blocks[i].plane.GetRigidBodyReference()->position.z - (150.0f*(float)levelBlocksCount));
		}
		level_blocks[i].Update(i_elapsedSecondCount_sinceLastUpdate);
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Game...");
	auto result = eae6320::Results::Success;

	//init gameObjects
	{
		if (!(result = ship.InitializeMeshEffect("data/Meshes/ship.json", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Failed Initializing GameObject")
			return result;
		}
	}
	shipCollider = Collision::cCollider(ship.GetRigidBodyReference(), Math::sVector(5,7,5));
	shipCollider.SetOnCollisionEnterCallback(OnCollisionEnter);
	shipCollider.SetOnCollisionExitCallback(OnCollisionExit);

	float zDist = -75;
	for(size_t i=0; i<levelBlocksCount; i++)
	{
		if (!(result = level_blocks[i].Initialize(zDist)))
		{
			EAE6320_ASSERTF(false, "Failed Initializing Level block")
			return result;
		}
		zDist -= 150;
	}
	
	return result;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Game...");

	ship.CleanUp();
	for(size_t i=0; i<levelBlocksCount; i++)
	{
		level_blocks[i].CleanUp();
	}
	return Results::Success;
}

void eae6320::cFinalGame::JumpStart()
{
	if(!isJumping)
	{
		Physics::sRigidBodyState* rigidBody = camera.GetRigidBodyReference();
		isJumping = true;
		currentJumpTime = 0;
		rigidBody->acceleration = Math::sVector(0,69,0);
	}
}

void eae6320::cFinalGame::JumpUpdate(const float i_elapsedSecondCount_sinceLastUpdate)
{
	Physics::sRigidBodyState* rigidBody = camera.GetRigidBodyReference();
	if(isJumping)
	{
		if(currentJumpTime < jumpTime)
		{
			currentJumpTime += i_elapsedSecondCount_sinceLastUpdate;
			rigidBody->acceleration = Math::sVector(0,69,0);
		}
		else
		{
			if(rigidBody->position.y <= 0)
			{
				isJumping = false;
			}
			rigidBody->acceleration = Math::sVector(0,-400,0);
		}
	}
	else
	{
		rigidBody->acceleration = Math::sVector(0,0,0);
	}
}

bool eae6320::cFinalGame::OnCollisionEnter(Collision::cCollider* self, Collision::cCollider* other)
{
	eae6320::Logging::OutputMessage("Collision Enter");
	other->GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(0,1,0);
	other->GetRigidBodyReference()->angularSpeed = 20;
	self->GetRigidBodyReference()->velocity = Math::sVector(0,0,0);
	isStopped = true;
	return true;
}

bool eae6320::cFinalGame::OnCollisionExit(Collision::cCollider* self,Collision::cCollider* other)
{
	eae6320::Logging::OutputMessage("Collision Exit");
	other->GetRigidBodyReference()->angularVelocity_axis_local = Math::sVector(0,1,0);
	other->GetRigidBodyReference()->angularSpeed = 0;
	isStopped = false;
	return true;
}
