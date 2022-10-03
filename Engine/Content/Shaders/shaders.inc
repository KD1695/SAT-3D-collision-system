/*
	This file should be #included by all shaders
*/

// DataTypes
//====================

#if defined( EAE6320_PLATFORM_GL )
	#define vector4 vec4
	#define vector2 vec2
	#define vector3 vec3
	#define vector4x4 mat4
#endif

#if defined( EAE6320_PLATFORM_D3D )
	#define vector4 float4
	#define vector2 float2
	#define vector3 float3
	#define vector4x4 float4x4
#endif

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif

// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define xproduct( m1, m2 ) mul(m1, m2)

#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define xproduct( m1, m2 ) m1 * m2

#endif

// Constant Buffers
//=================
DeclareConstantBuffer(g_constantBuffer_frame, 0)
{
	vector4x4 g_transform_worldToCamera;
	vector4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	vector2 g_padding;
};
DeclareConstantBuffer(g_constantBuffer_drawCall, 2)
{
	vector4x4 g_transform_localToWorld;
};