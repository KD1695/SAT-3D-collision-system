#pragma once
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>

namespace eae6320
{
	namespace Components
	{
		struct sVector3
		{
			int x = 0;
			int y = 0;
			int z = 0;
		};

		class GameObject
		{
			eae6320::Components::sVector3 position;
			eae6320::Graphics::cMesh* mesh = nullptr;
			eae6320::Graphics::cEffect* effect = nullptr;

		public:
			void SetPosition(sVector3 _position);
			sVector3 GetPosition();
			cResult InitializeMeshEffect(size_t indexCount, uint16_t indexData[], size_t vertexCount, eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[],
				std::string fragmentShaderPath = "data/Shaders/Fragment/standard.shader", std::string vertexShaderPath = "data/Shaders/Vertex/standard.shader");
			eae6320::Graphics::cMesh* GetMesh();
			eae6320::Graphics::cEffect* GetEffect();
			void CleanUp();
			~GameObject();
		};
	}
}