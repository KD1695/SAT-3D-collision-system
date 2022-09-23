#pragma once
#include <Engine/Results/cResult.h>
#include <Engine/Graphics/cVertexFormat.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Assets/ReferenceCountedAssets.h>

#ifdef EAE6320_PLATFORM_D3D
#include <Engine/Graphics/Direct3D/Includes.h>
#endif
#ifdef EAE6320_PLATFORM_GL
#include <Engine/Graphics/OpenGL/Includes.h>
#endif

namespace eae6320
{
	namespace Graphics
	{
		class cMesh
		{
			// Geometry Data
			//--------------
			size_t indexCount = 0;
#ifdef EAE6320_PLATFORM_D3D
			eae6320::Graphics::cVertexFormat* s_vertexFormat = nullptr;
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* s_vertexBuffer = nullptr;
			ID3D11Buffer* s_indexBuffer = nullptr;
#endif
#ifdef EAE6320_PLATFORM_GL
			// A vertex buffer holds the data for each vertex
			GLuint s_vertexBufferId = 0;
			GLuint s_indexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint s_vertexArrayId = 0;
#endif
			cMesh() = default;
			~cMesh();
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			cResult InitializeMesh(size_t indexCount, uint16_t indexData[], size_t vertexCount, eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[]);
			cResult Cleanup();

		public:
			void Draw();
			static cResult Load(cMesh*& o_mesh, size_t indexCount, uint16_t indexData[], size_t vertexCount, eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[]);
			// Reference Counting
			//-------------------
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMesh)
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()

		};
	}
}