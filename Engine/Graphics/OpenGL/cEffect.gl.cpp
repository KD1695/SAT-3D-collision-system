#include <Engine/Graphics/cEffect.h>
#include <Engine/Results/cResult.h>
#include <Engine/Results/Results.h>
#include <Engine/Logging/Logging.h>

void eae6320::Graphics::cEffect::Bind()
{
	{
		EAE6320_ASSERT(s_programId != 0);
		glUseProgram(s_programId);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Render state
	{
		s_renderState.Bind();
	}
}