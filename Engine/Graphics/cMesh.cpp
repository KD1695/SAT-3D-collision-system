#include <Engine/Graphics/cMesh.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <new>
#include <fstream>

eae6320::Graphics::cMesh::~cMesh()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = Cleanup();
	EAE6320_ASSERT(result);
}

eae6320::cResult eae6320::Graphics::cMesh::Load(cMesh*& o_mesh, size_t indexCount, uint16_t indexData[], size_t vertexCount, eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[])
{
	auto result = Results::Success;

	cMesh* newMesh = nullptr;
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				EAE6320_ASSERT(newMesh != nullptr);
				o_mesh = newMesh;
			}
			else
			{
				if (newMesh)
				{
					newMesh->DecrementReferenceCount();
					newMesh = nullptr;
				}
				o_mesh = nullptr;
			}
		});

	// Allocate a new mesh
	{
		newMesh = new (std::nothrow) cMesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			return result;
		}
	}
	// Initialize the platform-specific graphics API mesh object
	if (!(result = newMesh->InitializeMesh(indexCount, indexData, vertexCount, vertexData)))
	{
		EAE6320_ASSERTF(false, "Initialization of new mesh failed");
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Graphics::cMesh::LoadFromFile(cMesh*& o_mesh, const char* filePath)
{
	//Read json from path
	EAE6320_ASSERT(filePath != nullptr);
	std::ifstream ifs(filePath);
	json j;
	ifs >> j;

	//mesh data
	size_t vertexCount=0;
	size_t indexCount=0;
	uint16_t *indexData = nullptr;
	VertexFormats::sVertex_mesh *vertexData = nullptr;

	//get mesh data from json object
	auto itr = j.find("vertexPositionData");
	auto itr2 = j.find("vertexColorData");
	bool hasColor = itr2 != j.end();
	if (itr != j.end())
	{
		auto _vertexData = j["vertexPositionData"].get<std::vector<std::vector<float>>>();
		std::vector<std::vector<float>> _colorData;
		if(hasColor)
			_colorData = j["vertexColorData"].get<std::vector<std::vector<float>>>();
		vertexCount = _vertexData.size();
		vertexData = reinterpret_cast<VertexFormats::sVertex_mesh*>(calloc(vertexCount, sizeof(VertexFormats::sVertex_mesh)));
		for (size_t i = 0; i < vertexCount; i++)
		{
			vertexData[i].x = _vertexData[i][0];
			vertexData[i].y = _vertexData[i][1];
			vertexData[i].z = _vertexData[i][2];
			if (hasColor)
			{
				vertexData[i].r = (uint8_t)(255 * _colorData[i][0]);
				vertexData[i].g = (uint8_t)(255 * _colorData[i][1]);
				vertexData[i].b = (uint8_t)(255 * _colorData[i][2]);
				vertexData[i].a = (uint8_t)(255 * _colorData[i][3]);
			}
			else
			{
				vertexData[i].r = 255;
				vertexData[i].g = 255;
				vertexData[i].b = 255;
				vertexData[i].a = 255;
			}
		}
	}

	itr = j.find("indexData");
	if (itr != j.end())
	{
		auto _indexData = j["indexData"].get<std::vector<uint16_t>>();
		indexCount = _indexData.size();
		indexData = reinterpret_cast<uint16_t*>(calloc(indexCount, sizeof(uint16_t)));
		for (size_t i = 0; i < indexCount; i++)
		{
			indexData[i] = _indexData[i];
		}
	}

	//Load mesh
	return Load(o_mesh, indexCount, indexData, vertexCount, vertexData);
}
