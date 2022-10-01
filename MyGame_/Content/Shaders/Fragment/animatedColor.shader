/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )

// Constant Buffers
//=================

cbuffer g_constantBuffer_frame : register( b0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding;
};

cbuffer g_constantBuffer_drawCall : register( b2 )
{
  float4x4 g_transform_localToWorld;
};

// Entry Point
//============

void main(

	// Input
	//======

	in const float4 i_fragmentPosition : SV_POSITION,

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET

)
{
	float timeSine = sin(g_elapsedSecondCount_simulationTime);
	float variableValue =  timeSine < 0 ? timeSine+1 : timeSine;
	// Output random fading red
	o_color = float4(
		// RGB (color)
		variableValue, 0.0, 1.0,
		// Alpha (opacity)
		1.0 );
}

#elif defined( EAE6320_PLATFORM_GL )

// Constant Buffers
//=================

layout( std140, binding = 0 ) uniform g_constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For vec4 alignment
	vec2 g_padding;
};

layout( std140, binding = 2 ) uniform g_constantBuffer_drawCall
{
  mat4 g_transform_localToWorld;
};

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

// Entry Point
//============

void main()
{
	float timeSine = sin(g_elapsedSecondCount_simulationTime);
	float variableValue =  timeSine < 0 ? timeSine+1 : timeSine;
	// Output random fading red
	o_color = vec4(
		// RGB (color)
		variableValue, 0.0, 0.0,
		// Alpha (opacity)
		1.0 );
}

#endif
