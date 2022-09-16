#include <Engine/Graphics/cEffect.h>
#include <Engine/Logging/Logging.h>

eae6320::cResult eae6320::Graphics::cEffect::Initialize(std::string fragmentShaderPath, std::string vertexShaderPath)
{
	auto result = eae6320::Results::Success;

	if (!(result = eae6320::Graphics::cShader::Load(vertexShaderPath,
		s_vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = eae6320::Graphics::cShader::Load(fragmentShaderPath,
		s_fragmentShader, eae6320::Graphics::eShaderType::Fragment)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return result;
	}
	{
		constexpr auto renderStateBits = []
		{
			uint8_t renderStateBits = 0;

			eae6320::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthWriting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

			return renderStateBits;
		}();
		if (!(result = s_renderState.Initialize(renderStateBits)))
		{
			EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
			return result;
		}
	}

#ifdef EAE6320_PLATFORM_GL
	InitProgram(result);
#endif // EAE6320_PLATFORM_GL

	return result;
}

eae6320::cResult eae6320::Graphics::cEffect::CleanUp()
{
	auto result = Results::Success;
	
#ifdef EAE6320_PLATFORM_GL
	CleanUpProgram();
#endif

	if (s_vertexShader)
	{
		s_vertexShader->DecrementReferenceCount();
		s_vertexShader = nullptr;
	}
	if (s_fragmentShader)
	{
		s_fragmentShader->DecrementReferenceCount();
		s_fragmentShader = nullptr;
	}

	return result;
}