#include "Precompiled.h"
#include "RenderObjectComponent.h"

#include "GameWorld.h"
#include "RenderService.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
void RenderObjectComponent::Initialize()
{
	RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Register(this);
}

void RenderObjectComponent::Terminate()
{
	RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Unregister(this);
}

void RenderObjectComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("CastShadow"))
	{
		mCastShadow = value["CastShadow"].GetBool();
	}
}

bool RenderObjectComponent::CastShadow() const
{
	return false;
}
