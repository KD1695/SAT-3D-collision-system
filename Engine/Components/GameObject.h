#pragma once
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>

namespace eae6320
{
	namespace Components
	{
		class GameObject
		{
			eae6320::Physics::sRigidBodyState rigidBodyState;
			eae6320::Graphics::cMesh* mesh = nullptr;
			eae6320::Graphics::cEffect* effect = nullptr;
			
			eae6320::Math::sVector position;
			eae6320::Math::cMatrix_transformation transform;

		public:
			cResult InitializeMeshEffect(size_t indexCount, uint16_t indexData[], size_t vertexCount, eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[],
				std::string fragmentShaderPath = "data/Shaders/Fragment/standard.shader", std::string vertexShaderPath = "data/Shaders/Vertex/standard.shader");
			cResult InitializeMeshEffect(std::string meshPath, 
				std::string fragmentShaderPath = "data/Shaders/Fragment/standard.shader", std::string vertexShaderPath = "data/Shaders/Vertex/standard.shader");
			eae6320::Graphics::cMesh* GetMesh();
			void SetMesh(Graphics::cMesh* mesh);
			eae6320::Graphics::cEffect* GetEffect();
			void Update(float i_secondCountToIntegrate);
			void SetVelocity(Math::sVector velocity);
			Math::cMatrix_transformation GetTransform();
			void CleanUp();
			~GameObject();
		};
	}
}