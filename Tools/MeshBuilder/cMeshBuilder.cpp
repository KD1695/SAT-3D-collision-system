#include "cMeshBuilder.h"
#include <Tools/AssetBuildLibrary/Functions.h>

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = Results::Success;
	Platform::sDataFromFile dataFromFile;
	// Load the source file
	{
		std::string errorMessage;
		if (!(result = Platform::LoadBinaryFile(m_path_source, dataFromFile, &errorMessage)))
		{
			OutputErrorMessageWithFileInfo(m_path_source, "Failed to load shader source file: %s", errorMessage.c_str());
			return result;
		}
	}
	// Write the mesh file to disk
	{
		std::string errorMessage;
		if (!(result = eae6320::Platform::WriteBinaryFile(m_path_target, dataFromFile.data, dataFromFile.size, &errorMessage)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());
		}
	}
	return result;
}
