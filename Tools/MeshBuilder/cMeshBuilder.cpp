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
		uint16_t len = (uint16_t)_vertexData.size();
		fwrite((void*)&len, sizeof(uint16_t), 1, fileToWrite);
		float* vertexDataArray = (float*)calloc(len, sizeof(float) * 3);
		float* floatIterator = vertexDataArray;
		for (size_t i = 0; i < _vertexData.size(); i++)
		{
			*floatIterator = _vertexData[i][0];
			floatIterator++;
			*floatIterator = _vertexData[i][1];
			floatIterator++;
			*floatIterator = _vertexData[i][2];
			floatIterator++;
		}
		fwrite(vertexDataArray, sizeof(float), _vertexData.size() * 3, fileToWrite);
		free(vertexDataArray);
	}
	
	auto itr3 = j.find("vertexColorData");
	if (itr3 != j.end())
	{
		auto _vertexColorData = j["vertexColorData"].get<std::vector<std::vector<float>>>();
		uint16_t len = (uint16_t)_vertexColorData.size();
		fwrite((void*)&len, sizeof(uint16_t), 1, fileToWrite);
		float* vertexColorDataArray = (float*)calloc(len, sizeof(float) * 4);
		float* floatIterator = vertexColorDataArray;
		for (size_t i = 0; i < _vertexColorData.size(); i++)
		{
			*floatIterator = _vertexColorData[i][0];
			floatIterator++;
			*floatIterator = _vertexColorData[i][1];
			floatIterator++;
			*floatIterator = _vertexColorData[i][2];
			floatIterator++;
			*floatIterator = _vertexColorData[i][3];
			floatIterator++;
		}
		fwrite(vertexColorDataArray, sizeof(float), _vertexColorData.size() * 4, fileToWrite);
		free(vertexColorDataArray);
	}

	auto itr2 = j.find("indexData");
	if (itr2 != j.end())
	{
		auto _indexData = j["indexData"].get<std::vector<uint16_t>>();
		uint16_t len = (uint16_t)_indexData.size();
		fwrite((void*)&len, sizeof(uint16_t), 1, fileToWrite);
		fwrite((void*)_indexData.data(), sizeof(_indexData[0]), _indexData.size(), fileToWrite);
	}

	fclose(fileToWrite);

	return result;
}
