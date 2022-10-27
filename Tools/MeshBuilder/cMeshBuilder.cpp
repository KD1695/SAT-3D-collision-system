#include "cMeshBuilder.h"
#include <fstream>
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

	json j = json::parse((char*)dataFromFile.data, ((char*)dataFromFile.data) + dataFromFile.size);
	void* dataToWrite = nullptr;
	size_t writeSize = 0;
	FILE* fileToWrite = fopen(m_path_target, "w");

	auto itr = j.find("vertexPositionData");
	if (itr != j.end())
	{
		auto _vertexData = j["vertexPositionData"].get<std::vector<std::vector<float>>>();
		size_t len = _vertexData.size();
		fwrite((void*)&len, sizeof(size_t), 1, fileToWrite);
		fwrite((void*)_vertexData.data(), sizeof(_vertexData[0]), _vertexData.size(), fileToWrite);
	}
	auto itr2 = j.find("indexData");
	if (itr2 != j.end())
	{
		auto _indexData = j["indexData"].get<std::vector<float>>();
		size_t len = _indexData.size();
		fwrite((void*)&len, sizeof(size_t), 1, fileToWrite);
		fwrite((void*)_indexData.data(), sizeof(_indexData[0]), _indexData.size(), fileToWrite);
	}

	fclose(fileToWrite);

	return result;
}
