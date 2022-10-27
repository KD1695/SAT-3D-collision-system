#pragma once
/*
	This class builds shaders
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Includes
//=========
#include <Tools/AssetBuildLibrary/iBuilder.h>
#include <Engine/Graphics/Configuration.h>
#include <Engine/Results/cResult.h>
#include <Engine/Platform/Platform.h>
#include <External/JSON/Includes.h>

// Class Declaration
//==================

namespace eae6320
{
	using json = nlohmann::json;

	namespace Assets
	{
		class cMeshBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build(const std::vector<std::string>& i_arguments) final;
		};
	}
}

#endif	// EAE6320_CSHADERBUILDER_H