#pragma once
#ifdef EAE6320_PLATFORM_D3D
#include <Engine/Graphics/Direct3D/Includes.h>
#endif
#ifdef EAE6320_PLATFORM_GL
#include <Engine/Graphics/OpenGL/Includes.h>
#endif
#include <Engine/Graphics/cRenderState.h>
#include <Engine/Graphics/cShader.h>

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{
#ifdef EAE6320_PLATFORM_GL
			GLuint s_programId = 0;
			cResult CleanUpProgram();
			cResult InitProgram(cResult result);
#endif
			eae6320::Graphics::cShader* s_vertexShader = nullptr;
			eae6320::Graphics::cShader* s_fragmentShader = nullptr;

			eae6320::Graphics::cRenderState s_renderState;

		public:
			cResult Initialize();
			cResult CleanUp();
			void Bind();
		};
	}
}