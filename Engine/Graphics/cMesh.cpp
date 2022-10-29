#include <Engine/Graphics/cMesh.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <new>
#include <fstream>
#include <Engine/Platform/Platform.h>

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
	
	//mesh data
	size_t vertexCount=0;
	size_t indexCount=0;
	uint16_t *indexData = nullptr;
	VertexFormats::sVertex_mesh *vertexData = nullptr;

	void* meshData = nullptr;
	char* meshDataIterator = nullptr;
	eae6320::Platform::sDataFromFile fileData;
	eae6320::Platform::LoadBinaryFile(filePath, fileData);
	meshDataIterator = (char*)fileData.data;
	
	meshData = malloc(sizeof(uint16_t));
	memcpy(meshData, meshDataIterator, sizeof(uint16_t));
	vertexCount = *reinterpret_cast<uint16_t*>(meshData);
	meshDataIterator += sizeof(uint16_t);

	meshData = malloc(sizeof(float) * 3 * (size_t)vertexCount);
	memcpy(meshData, meshDataIterator, sizeof(float) * 3 * (size_t)vertexCount);
	float* _vertexData = reinterpret_cast<float*>(meshData);
	meshDataIterator += 4 * 3 * (size_t)vertexCount;

	meshData = malloc(sizeof(uint16_t));
	memcpy(meshData, meshDataIterator, sizeof(uint16_t));
	size_t newCount = *reinterpret_cast<uint16_t*>(meshData);
	bool hasColor = vertexCount == newCount;
	meshDataIterator += sizeof(uint16_t);

	float* _colorData = nullptr;
	if (hasColor)
	{
		meshData = malloc(sizeof(float) * 4 * (size_t)vertexCount);
		memcpy(meshData, meshDataIterator, sizeof(float) * 4 * (size_t)vertexCount);
		_colorData = reinterpret_cast<float*>(meshData);
		meshDataIterator += sizeof(float) * 4 * (size_t)vertexCount;
		
		meshData = malloc(sizeof(uint16_t));
		memcpy(meshData, meshDataIterator, sizeof(uint16_t));
		indexCount = (size_t)*reinterpret_cast<uint16_t*>(meshData);
		meshDataIterator += sizeof(uint16_t);
	}
	else
	{
		indexCount = newCount;
	}

	meshData = malloc(sizeof(uint16_t) * (size_t)indexCount);
	memcpy(meshData, meshDataIterator, sizeof(uint16_t) * (size_t)indexCount);
	indexData = reinterpret_cast<uint16_t*>(meshData);
	
	vertexData = reinterpret_cast<VertexFormats::sVertex_mesh*>(calloc((size_t)vertexCount, sizeof(VertexFormats::sVertex_mesh)));
	for (size_t i = 0; i < (size_t)vertexCount; i++)
	{
		vertexData[i].x = _vertexData[i * 3 + 0];
		vertexData[i].y = _vertexData[i * 3 + 1];
		vertexData[i].z = _vertexData[i * 3 + 2];
		if (hasColor)
		{
			vertexData[i].r = (uint8_t)(255 * _colorData[i * 4 + 0]);
			vertexData[i].g = (uint8_t)(255 * _colorData[i * 4 + 1]);
			vertexData[i].b = (uint8_t)(255 * _colorData[i * 4 + 2]);
			vertexData[i].a = (uint8_t)(255 * _colorData[i * 4 + 3]);
		}
		else
		{
			vertexData[i].r = 255;
			vertexData[i].g = 255;
			vertexData[i].b = 255;
			vertexData[i].a = 255;
		}
	}

	//Load mesh
	return Load(o_mesh, indexCount, indexData, vertexCount, vertexData);
}
