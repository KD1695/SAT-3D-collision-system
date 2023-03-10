/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )
// Entry Point
//============

void main(

	// Input
	//======

	in const vector4 i_fragmentPosition : SV_POSITION,
	in const vector4 i_fragmentColor : COLOR,

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out vector4 o_color : SV_TARGET

)

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======
layout( location = 1 ) in vector4 i_fragmentColor;

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vector4 o_color;

// Entry Point
//============

void main()

#endif
//shader code body
{
	float timeSine = sin(g_elapsedSecondCount_simulationTime);
	float variableValue =  timeSine < 0 ? timeSine+1 : timeSine;
	// Output random fading red
	o_color = i_fragmentColor * vector4(
		// RGB (color)
		variableValue, 0.0, 0.0,
		// Alpha (opacity)
		1.0 );
}