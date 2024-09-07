#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"
#include "Animator.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void Graphics::RenderObject::Terminate()
{
	meshBuffer.Terminate();
	diffuseMapId = 0;
	normalMapId = 0;
	specMapId = 0;
	bumpMapId = 0;
}
RenderGroup Graphics::CreateRenderGroup(ModelId id, const Animator* animator)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	ASSERT(model != nullptr, "RenderGroup: ModelID %d is not loaded", id);
	RenderGroup renderGroup = CreateRenderGroup(*model,animator);
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.modelId = id;
	}
	return renderGroup;
}
RenderGroup Graphics::CreateRenderGroup(const Model& model, const Animator* animator)
{
	auto TryLoadTexture = [](const auto& textureName)->TextureId
	{
		if (textureName.empty())
		{
			return 0;
		}
		return TextureManager::Get()->LoadTexture(textureName, false);
	};
	RenderGroup renderGroup;
	for (const Model::MeshData& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		renderObject.meshBuffer.Initialize(meshData.mesh);
		if (meshData.materialIndex< model.materialData.size())
		{
			const Model::MaterialData& materialData = model.materialData[meshData.materialIndex];
			renderObject.material = materialData.material;
			renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseMapName);
			renderObject.normalMapId = TryLoadTexture(materialData.normalMapName);
			renderObject.bumpMapId = TryLoadTexture(materialData.bumpMapName);
			renderObject.specMapId = TryLoadTexture(materialData.specularMapName);
		}
		renderObject.skeleton = model.skeleton.get();
		renderObject.animator = animator;
	}
	return renderGroup;
}
void Graphics::CleanupRenderGroup(RenderGroup& renderGroup)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}

void Graphics::SetRenderGroupPosition(RenderGroup& renderGroup, const Math::Vector3& position)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.transform.position = position;
	}
}

