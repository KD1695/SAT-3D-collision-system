#include "LevelBlock.h"

LevelBlock::LevelBlock()
{
}

LevelBlock::LevelBlock(std::string _blockFragmentShader, std::string _blockVertexShader,
    std::string _planeFragmentShader, std::string _planeVertexShader, int _blockCount)
{
    blockCount = _blockCount;
    blockFragmentShader = _blockFragmentShader;
    blockVertexShader = _blockVertexShader;
    planeFragmentShader = _planeFragmentShader;
    planeVertexShader = _planeVertexShader;
}

eae6320::cResult LevelBlock::Initialize(float zDist)
{
    auto result = eae6320::Results::Success;

    if (!(result = plane.InitializeMeshEffect(planeMesh, planeFragmentShader, planeVertexShader)))
    {
        EAE6320_ASSERTF(false, "Failed Initializing Plane in LevelBlock")
        return result;
    }
    plane.GetRigidBodyReference()->position = eae6320::Math::sVector(0, -10, zDist);

    for(int i=0; i<blockCount; i++)
    {
        if (!(result = blocks[i].InitializeMeshEffect(blockMesh, blockFragmentShader, blockVertexShader)))
        {
            EAE6320_ASSERTF(false, "Failed Initializing Blocks in LevelBlock")
            return result;
        }
        srand(blockCount+i+(int)zDist);
        float x = (float)(((rand()%3)-1)*10);
        float z = (float)(rand()%130)-65;
        blocks[i].GetRigidBodyReference()->position = eae6320::Math::sVector(x, plane.GetRigidBodyReference()->position.y, plane.GetRigidBodyReference()->position.z + z);
        colliders[i] = eae6320::Collision::cCollider(blocks[i].GetRigidBodyReference(),eae6320::Math::sVector(7,7,7));
    }

    isInitialized = true;
    return eae6320::Results::Success;
}

void LevelBlock::CleanUp()
{
    if(!isInitialized)
        return;
    for(int i=0; i<blockCount; i++)
    {
        blocks[i].CleanUp();
    }
    plane.CleanUp();
}

void LevelBlock::Update(const float i_secondCountToIntegrate)
{
    if(!isInitialized)
        return;
    
    for(int i=0; i<blockCount; i++)
    {
        blocks[i].Update(i_secondCountToIntegrate);
        colliders[i].Update(i_secondCountToIntegrate);
    }
    plane.Update(i_secondCountToIntegrate);
}

void LevelBlock::UpdatePosition(float zDist)
{
    plane.GetRigidBodyReference()->position = eae6320::Math::sVector(0, -10, zDist);

    for(int i=0; i<blockCount; i++)
    {
        srand(blockCount+i+(int)zDist);
        float x = (float)(((rand()%3)-1)*10);
        float z = (float)(rand()%130)-65;
        blocks[i].GetRigidBodyReference()->position = eae6320::Math::sVector(x, plane.GetRigidBodyReference()->position.y, plane.GetRigidBodyReference()->position.z + z);
        blocks[i].GetRigidBodyReference()->angularSpeed = 0;
    }
}

int LevelBlock::GetGameObjects(eae6320::Components::GameObject o_game_objects[], int startIndex)
{
    if(!isInitialized)
        return 0;
    o_game_objects[startIndex] = plane;
    for(int i=startIndex; i<startIndex+blockCount; i++)
    {
        o_game_objects[i+1] = blocks[i-startIndex];
    }
    return blockCount+1;
}