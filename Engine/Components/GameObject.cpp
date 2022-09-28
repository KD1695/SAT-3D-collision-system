#include <Engine/Components/GameObject.h>

void eae6320::Components::GameObject::SetPosition(sVector3 _position)
{
	position.x = _position.x;
	position.y = _position.y;
	position.z = _position.z;
}

eae6320::Components::GameObject::~GameObject()
{
	CleanUp();
}

eae6320::Components::sVector3 eae6320::Components::GameObject::GetPosition()
{
	return position;
}

eae6320::cResult eae6320::Components::GameObject::InitializeMeshEffect(size_t indexCount, uint16_t indexData[], size_t vertexCount, eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[], 
	std::string fragmentShaderPath, std::string vertexShaderPath)
{
	auto result = Results::Success;

	//load effect
	{
		if (!(result = eae6320::Graphics::cEffect::Load(effect, fragmentShaderPath, vertexShaderPath)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}

	//load mesh
	{
		if (!(result = eae6320::Graphics::cMesh::Load(mesh, indexCount, indexData, vertexCount, vertexData)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}

	return result;
}

void eae6320::Components::GameObject::CleanUp()
{
	if (effect != nullptr)
	{
		effect->DecrementReferenceCount();
		effect = nullptr;
	}

	if (mesh != nullptr)
	{
		mesh->DecrementReferenceCount();
		mesh = nullptr;
	}
}

eae6320::Graphics::cMesh* eae6320::Components::GameObject::GetMesh()
{
	return mesh;
}

eae6320::Graphics::cEffect* eae6320::Components::GameObject::GetEffect()
{
	return effect;
}