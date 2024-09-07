#include"Precompiled.h"
#include"ModelComponent.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void ModelComponent::Initialize()
{
	ModelManager* mm = ModelManager::Get();
	mModelId = mm->GetModelId(mFileName);
	if (mm->GetModel(mModelId) == nullptr)
	{
		mModelId = mm->LoadModel(mFileName);
		for (const std::string& fileName : mAnimationFileNames)
		{
			mm->AddAnimation(mModelId, fileName);
		}
	}

	ASSERT(mm->GetModel(mModelId) != nullptr, "ModelComponent: model is null");
	RenderObjectComponent::Initialize();
}

void ModelComponent::Terminate()
{
	RenderObjectComponent::Terminate();
}

void ModelComponent::Deserialize(const rapidjson::Value& value)
{
	RenderObjectComponent::Deserialize(value);
	if (value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
	if (value.HasMember("Animation"))
	{
		mAnimationFileNames.clear();
		auto animations = value["Animation"].GetArray();
		for (auto& animation : animations)
		{
			mAnimationFileNames.push_back(animation.GetString());
		}
	}
}
Graphics::ModelId ModelComponent::GetModelId() const
{
	return mModelId;
}

const Graphics::Model& SpringEngine::ModelComponent::GetModel() const
{
	return *ModelManager::Get()->GetModel(mModelId);
}
