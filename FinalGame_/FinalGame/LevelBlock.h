#pragma once
#include "Engine/Components/GameObject.h"
#include <Engine/Results/cResult.h>

#include "Engine/Collision/Collider.h"

class LevelBlock
{
    const std::string blockMesh = "data/Meshes/cube.json";
    std::string blockFragmentShader = "data/Shaders/Fragment/standard.shader";
    std::string blockVertexShader = "data/Shaders/Vertex/standard.shader";

    const std::string planeMesh = "data/Meshes/plane.json";
    std::string planeFragmentShader = "data/Shaders/Fragment/standard.shader";
    std::string planeVertexShader = "data/Shaders/Vertex/standard.shader";
    const int planeLength = 150;
    const int planeWidth = 30;
    
    bool isInitialized = false;
    
public:
    int blockCount = 4;
    eae6320::Components::GameObject blocks[4];
    eae6320::Collision::cCollider colliders[4];
    eae6320::Components::GameObject plane;

    LevelBlock();
    LevelBlock(std::string _blockFragmentShader, std::string _blockVertexShader, std::string _planeFragmentShader, std::string _planeVertexShader, int _blockCount);
    eae6320::cResult Initialize(float zDist);
    void CleanUp();
    void Update(const float i_secondCountToIntegrate);
    void UpdatePosition(float zDist);
    int GetGameObjects(eae6320::Components::GameObject o_game_objects[], int startIndex);
};
