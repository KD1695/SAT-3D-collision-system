/*
	This vertex shader is used to create a Direct3D vertex input layout object
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )
// Entry Point
//============

void main(

	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateInputLayout()

	// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code
	in const vector3 i_position : POSITION,
	in const vector4 i_color : COLOR,

	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out vector4 o_position : SV_POSITION,
	out vector4 o_color : COLOR

)
{
	// The shader program is only used to generate a vertex input layout object;
	// the actual shading code is never used
	o_position = vector4( i_position, 1.0 );
	o_color = i_color;
}

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======

// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()

// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code
layout( location = 0 ) in vector3 i_position;
layout( location = 1 ) in vector4 i_color;

// Output
//=======

// The vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an automatically-required variable named "gl_Position"
layout( location = 1 ) out vector4 o_color;

// Entry Point
//============

void main()
{
	// The shader program is only used by Direct3D
	gl_Position = vector4( i_position, 1.0 );
	o_color = i_color;
}
#endif
