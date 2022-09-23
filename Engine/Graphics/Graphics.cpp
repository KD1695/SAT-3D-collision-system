// Includes
//=========

#include <Engine/Graphics/Graphics.h>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cConstantBuffer.h>
#include <Engine/Graphics/ConstantBufferFormats.h>

// Static Data
//============

namespace
{
	struct sInitializationParameters;
	// Constant buffer object
	eae6320::Graphics::cConstantBuffer s_constantBuffer_frame(eae6320::Graphics::ConstantBufferTypes::Frame);
	
	// Submission Data
	//----------------

	// This struct's data is populated at submission time;
	// it must cache whatever is necessary in order to render a frame
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sFrame constantData_frame;
		float bgColor[4];
		eae6320::Graphics::cMesh* mesh1 = nullptr;
		eae6320::Graphics::cEffect* effect1 = nullptr;
	};
	// In our class there will be two copies of the data required to render a frame:
	//	* One of them will be in the process of being populated by the data currently being submitted by the application loop thread
	//	* One of them will be fully populated and in the process of being rendered from in the render thread
	// (In other words, one is being produced while the other is being consumed)
	sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
	auto* s_dataBeingSubmittedByApplicationThread = &s_dataRequiredToRenderAFrame[0];
	auto* s_dataBeingRenderedByRenderThread = &s_dataRequiredToRenderAFrame[1];
	// The following two events work together to make sure that
	// the main/render thread and the application loop thread can work in parallel but stay in sync:
	// This event is signaled by the application loop thread when it has finished submitting render data for a frame
	// (the main/render thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
	// This event is signaled by the main/render thread when it has swapped render data pointers.
	// This means that the renderer is now working with all the submitted data it needs to render the next frame,
	// and the application loop thread can start submitting data for the following frame
	// (the application loop thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;
}

// Interface
//==========

// Submission
//-----------

void eae6320::Graphics::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constantData_frame = s_dataBeingSubmittedByApplicationThread->constantData_frame;
	constantData_frame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_frame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}

eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}

eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}

// Render
//-------

void eae6320::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		if (Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread))
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread);
			// Once the pointers have been swapped the application loop can submit new data
			if (!s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal())
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
				Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
			Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
	}

	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread);
	auto* const dataRequiredToRenderFrame = s_dataBeingRenderedByRenderThread;

	Graphics::SetupBuffer(dataRequiredToRenderFrame->bgColor);
	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_frame = dataRequiredToRenderFrame->constantData_frame;
		s_constantBuffer_frame.Update(&constantData_frame);
	}

	// Bind the shading data
	if (dataRequiredToRenderFrame->effect1 != nullptr)
		dataRequiredToRenderFrame->effect1->Bind();
	// Draw the geometry
	if (dataRequiredToRenderFrame->mesh1 != nullptr)
		dataRequiredToRenderFrame->mesh1->Draw();

	Graphics::SwapBuffer();

	// After all of the data that was submitted for this frame has been used
	// you must make sure that it is all cleaned up and cleared out
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		if (dataRequiredToRenderFrame->effect1 != nullptr)
		{
			dataRequiredToRenderFrame->effect1->DecrementReferenceCount();
			dataRequiredToRenderFrame->effect1 = nullptr;
		}

		if (dataRequiredToRenderFrame->mesh1 != nullptr)
		{
			dataRequiredToRenderFrame->mesh1->DecrementReferenceCount();
			dataRequiredToRenderFrame->mesh1 = nullptr;
		}
	}
}

void eae6320::Graphics::SetBgColor(float color[4])
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);

	s_dataBeingSubmittedByApplicationThread->bgColor[0] = color[0];
	s_dataBeingSubmittedByApplicationThread->bgColor[1] = color[1];
	s_dataBeingSubmittedByApplicationThread->bgColor[2] = color[2];
	s_dataBeingSubmittedByApplicationThread->bgColor[3] = color[3];
}

void eae6320::Graphics::SetMeshData(eae6320::Graphics::cMesh* mesh)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);

	s_dataBeingSubmittedByApplicationThread->mesh1 = mesh;
	s_dataBeingSubmittedByApplicationThread->mesh1->IncrementReferenceCount();
}

void eae6320::Graphics::SetEffectData(eae6320::Graphics::cEffect* effect)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);

	s_dataBeingSubmittedByApplicationThread->effect1 = effect;
	s_dataBeingSubmittedByApplicationThread->effect1->IncrementReferenceCount();
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without context");
		return result;
	}
	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_frame.Initialize())
		{
			// There is only a single frame constant buffer that is reused
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_frame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without frame constant buffer");
			return result;
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data has been submitted from the application thread");
			return result;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data can be submitted from the application thread");
			return result;
		}
	}

	Graphics::InitializeBuffer(i_initializationParameters);

	return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = Results::Success;

	Graphics::CleanUpBuffer();

	if (s_dataBeingSubmittedByApplicationThread->effect1 != nullptr)
	{
		s_dataBeingSubmittedByApplicationThread->effect1->DecrementReferenceCount();
		s_dataBeingSubmittedByApplicationThread->effect1 = nullptr;
	}
	if (s_dataBeingSubmittedByApplicationThread->mesh1 != nullptr)
	{
		s_dataBeingSubmittedByApplicationThread->mesh1->DecrementReferenceCount();
		s_dataBeingSubmittedByApplicationThread->mesh1 = nullptr;
	}

	if (s_dataBeingRenderedByRenderThread->effect1 != nullptr)
	{
		s_dataBeingRenderedByRenderThread->effect1->DecrementReferenceCount();
		s_dataBeingRenderedByRenderThread->effect1 = nullptr;
	}
	if (s_dataBeingRenderedByRenderThread->mesh1 != nullptr)
	{
		s_dataBeingRenderedByRenderThread->mesh1->DecrementReferenceCount();
		s_dataBeingRenderedByRenderThread->mesh1 = nullptr;
	}

	{
		const auto result_constantBuffer_frame = s_constantBuffer_frame.CleanUp();
		if (!result_constantBuffer_frame)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_frame;
			}
		}
	}

	{
		const auto result_context = sContext::g_context.CleanUp();
		if (!result_context)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_context;
			}
		}
	}

	return result;
}