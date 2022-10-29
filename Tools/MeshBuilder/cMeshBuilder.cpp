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
	std::ofstream meshFile(m_path_target, std::ofstream::binary);

	auto itr = j.find("vertexPositionData");
	if (itr != j.end())
	{
		auto _vertexData = j["vertexPositionData"].get<std::vector<std::vector<float>>>();
		uint16_t len = (uint16_t)_vertexData.size();
		meshFile.write((char*)&len, sizeof(uint16_t));
		float* vertexDataArray = (float*)malloc(len * 4 * 3);
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
		meshFile.write((char*)vertexDataArray, sizeof(float) * len * 3);
		free(vertexDataArray);
	}
	
	auto itr3 = j.find("vertexColorData");
	if (itr3 != j.end())
	{
		auto _vertexColorData = j["vertexColorData"].get<std::vector<std::vector<float>>>();
		uint16_t len = (uint16_t)_vertexColorData.size();
		meshFile.write((char*)&len, sizeof(uint16_t));
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
		meshFile.write((char*)vertexColorDataArray, sizeof(float) * _vertexColorData.size() * 4);
		free(vertexColorDataArray);
	}

	auto itr2 = j.find("indexData");
	if (itr2 != j.end())
	{
		auto _indexData = j["indexData"].get<std::vector<uint16_t>>();
		uint16_t len = (uint16_t)_indexData.size();
		meshFile.write((char*)&len, sizeof(uint16_t));
		meshFile.write((char*)_indexData.data(), sizeof(_indexData[0]) * _indexData.size());
	}

	meshFile.close();

	return result;
}
