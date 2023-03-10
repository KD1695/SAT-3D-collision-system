--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/animatedColor.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/newColor.shader", arguments = { "fragment" } },
		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},

	meshes = 
	{
		"Meshes/square.json",
		"Meshes/triangle.json",
		"Meshes/pyramid.json",
		"Meshes/pyramid_color.json",
		"Meshes/helix.json",
		"Meshes/torus.json",
		"Meshes/plane.json",
		"Meshes/cube.json"
	},
}
