#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "FireworkParticleSystem.h"

#include "Event.h"
#include "EventManager.h"

#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"
#include "SaveUtil.h"

// components
#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "Component.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "RenderObjectComponent.h"
#include "RigidBodyComponent.h"
#include "SoundBankComponent.h"
#include "SoundEffectComponent.h"
#include "TransformComponent.h"
#include "UIComponent.h"
#include "UIButtonComponent.h"
#include "UISpriteComponent.h"
#include "UITextComponent.h"

// services
#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"
#include "UpdateService.h"
#include "PhysicsService.h"
#include "UIRenderService.h"

namespace SpringEngine
{
	App& MainApp();
}