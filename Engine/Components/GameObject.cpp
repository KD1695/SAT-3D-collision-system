#include <Engine/Components/GameObject.h>
#include <Engine/Logging/Logging.h>

eae6320::Components::GameObject::~GameObject()
{
	CleanUp();
}

void eae6320::Components::GameObject::SetVelocity(Math::sVector velocity)
{
	rigidBodyState.velocity = velocity;
}

eae6320::Math::cMatrix_transformation eae6320::Components::GameObject::GetTransform()
{
	return transform;
}

eae6320::cResult eae6320::Components::GameObject::InitializeMeshEffect(size_t _indexCount, uint16_t _indexData[], size_t _vertexCount, eae6320::Graphics::VertexFormats::sVertex_mesh _vertexData[],
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
		if (!(result = eae6320::Graphics::cMesh::Load(mesh, _indexCount, _indexData, _vertexCount, _vertexData)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}
	//save mesh geometry data
	indexCount = _indexCount;
	indexData = _indexData;
	vertexCount = _vertexCount;
	vertexData = _vertexData;

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

void eae6320::Components::GameObject::Update(const float i_secondCountToIntegrate)
{
	transform = rigidBodyState.PredictFutureTransform(i_secondCountToIntegrate);
	rigidBodyState.Update(i_secondCountToIntegrate);
}

eae6320::Graphics::cMesh* eae6320::Components::GameObject::GetMesh()
{
	return mesh;
}

eae6320::Graphics::cEffect* eae6320::Components::GameObject::GetEffect()
{
	return effect;
}