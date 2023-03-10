--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},
	
	meshes = 
    	{
    		"Meshes/cube.json",
    		"Meshes/plane.json",
    		"Meshes/ship.json"
        },

	audios =
    {
        { path = "Audios/collide.mp3"},
        { path = "Audios/move.mp3"},
        { path = "Audios/bgm.mp3"},
    },
}
