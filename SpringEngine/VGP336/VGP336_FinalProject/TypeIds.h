#pragma once


#include <SpringEngine/Inc/SpringEngine.h>

enum class CustomComponentId
{
	CustomDebugDraw = static_cast<uint32_t>(SpringEngine::ComponentId::Count)
};

enum class CustomServiceId
{
	CustomDebugDrawDisplay = static_cast<uint32_t>(SpringEngine::ServiceId::Count)
};