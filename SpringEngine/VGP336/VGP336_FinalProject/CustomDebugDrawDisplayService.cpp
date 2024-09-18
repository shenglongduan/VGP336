#include "CustomDebugDrawDisplayService.h"
#include "CustomDebugDrawComponent.h"

void CustomDebugDrawDisplayService::Render()
{
	for (CustomDebugDrawComponent* debugDrawItem : mCustomDebugDrawComponents)
	{
		debugDrawItem->AddDebugDraw();
	}
}

void CustomDebugDrawDisplayService::Register(CustomDebugDrawComponent* debugDrawComponent)
{
	auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), debugDrawComponent);
	if (iter==mCustomDebugDrawComponents.end())
	{
		mCustomDebugDrawComponents.push_back(debugDrawComponent);
	}
}

void CustomDebugDrawDisplayService::Unregister(CustomDebugDrawComponent* debugDrawComponent)
{
	auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), debugDrawComponent);
	if (iter != mCustomDebugDrawComponents.end())
	{
		mCustomDebugDrawComponents.erase(iter);
	}
}
