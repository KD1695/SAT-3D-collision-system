#include <Engine/Graphics/cEffect.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>

eae6320::Graphics::cEffect::~cEffect()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}

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

			eae6320::Graphics::RenderStates::EnableAlphaTransparency(renderStateBits);
			eae6320::Graphics::RenderStates::EnableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::EnableDepthWriting(renderStateBits);
			eae6320::Graphics::RenderStates::EnableDrawingBothTriangleSides(renderStateBits);

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

eae6320::cResult eae6320::Graphics::cEffect::Load(cEffect*& o_effect, std::string fragmentShaderPath, std::string vertexShaderPath)
{
	auto result = Results::Success;

	cEffect* newEffect = nullptr;
	cScopeGuard scopeGuard([&o_effect, &result, &newEffect]
		{
			if (result)
			{
				EAE6320_ASSERT(newEffect != nullptr);
				o_effect = newEffect;
			}
			else
			{
				if (newEffect)
				{
					newEffect->DecrementReferenceCount();
					newEffect = nullptr;
				}
				o_effect = nullptr;
			}
		});

	// Allocate a new effect
	{
		newEffect = new (std::nothrow) cEffect();
		if (!newEffect)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the effect");
			Logging::OutputError("Failed to allocate memory for the effect");
			return result;
		}
	}
	// Initialize the platform-specific graphics API effect object
	if (!(result = newEffect->Initialize(fragmentShaderPath, vertexShaderPath)))
	{
		EAE6320_ASSERTF(false, "Initialization of new effect failed");
		return result;
	}

	return result;
}