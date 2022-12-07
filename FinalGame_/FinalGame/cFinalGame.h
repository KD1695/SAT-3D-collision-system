/*
	This class is your specific game
*/

#ifndef EAE6320_CFINALGAME_H
#define EAE6320_CFINALGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>

#include "LevelBlock.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/GameObject.h"
#include "Engine/Audio/AudioSystem.h"

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif

// Class Declaration
//==================

namespace eae6320
{
	class cFinalGame final : public Application::iApplication
	{
		// Inherited Implementation
		//=========================

	private:

		float bg_Color[4] = { 0,1,1,1 };
		Components::GameObject* allGameObjects = nullptr;
		
		Components::Camera camera;
		Components::GameObject ship;
		Collision::cCollider shipCollider;
		LevelBlock level_blocks[3];
		size_t levelBlocksCount = 3;

		eae6320::AudioSystem::cAudio bgAudio;
		eae6320::AudioSystem::cAudio moveAudio;
		inline static eae6320::AudioSystem::cAudio collisionAudio;

		//movement
		float shipSpeed = 80.0f;
		float movementDistance = 10.0f;
		float lanesOnEachSide = 1;
		bool isTargetSet = false;
		float currentTargetXPos = 0.0f;
		bool isLeftKeyPressed = false;
		bool isRightKeyPressed = false;
		bool isJumping = false;
		float jumpTime = 0.75f;
		float currentJumpTime = 0.0f;

		inline static bool isStopped = false;
		
		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "KD's EAE6320 Final Game"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
			;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "KD's EAE6320 Final Game Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_VSDEFAULT_LARGE; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_VSDEFAULT_SMALL; return &iconId_small; }
#endif

		// Run
		//----

		void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate);
		void UpdateSimulationBasedOnInput() final;
		void UpdateBasedOnInput() final;
		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) final;

		// Initialize / Clean Up
		//----------------------

		cResult Initialize() final;
		cResult CleanUp() final;

		//GameFunctions
		void JumpStart();
		void JumpUpdate(const float i_elapsedSecondCount_sinceLastUpdate);

		//Collision Callbacks
		//----
		static bool OnCollisionEnter(Collision::cCollider* self, Collision::cCollider* other);
		static bool OnCollisionExit(Collision::cCollider* self, Collision::cCollider* other);
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CFINALGAME_H
