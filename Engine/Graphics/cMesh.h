#pragma once
#include <Engine/Results/cResult.h>
#include <Engine/Graphics/cVertexFormat.h>

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
#ifdef EAE6320_PLATFORM_D3D
			eae6320::Graphics::cVertexFormat* s_vertexFormat = nullptr;
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* s_vertexBuffer = nullptr;
#endif
#ifdef EAE6320_PLATFORM_GL
			// A vertex buffer holds the data for each vertex
			GLuint s_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint s_vertexArrayId = 0;
#endif

		public:
			cResult InitializeMesh();
			cResult Cleanup();
			void Draw();

			cMesh() = default;
			~cMesh();
		};
	}
}