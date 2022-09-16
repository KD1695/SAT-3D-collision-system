#pragma once
#include <Engine/Results/cResult.h>
#include <Engine/Graphics/sContext.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>

namespace eae6320
{
	namespace Graphics
	{
		struct sInitializationParameters;

		cResult InitializeBuffer(const sInitializationParameters& i_initializationParameters);
		void SetupBuffer();
		void SwapBuffer();
		void CleanUpBuffer();
	}
}