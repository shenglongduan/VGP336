#include"GameState.h"

#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawDisplayService.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

namespace
{
	Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
	{
		if (componentName == "CustomDebugDrawComponent")
		{
			return gameObject.AddComponent<CustomDebugDrawComponent>();

		}
		return nullptr;
	}
	Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameworld)
	{
		if (serviceName == "CustomDebugDrawDisplayService")
		{
			return gameworld.AddService<CustomDebugDrawDisplayService>();

		}
		return nullptr;
	}
}
void GameState::Initialize()
{
	GameObjectFactory::SetCustomMake(CustomComponentMake);
	GameWorld::SetCustomService(CustomServiceMake);
	mGameWorld.LoadLevel("../../Assets/Templates/Level/test_level.json");
}
void GameState::Terminate()
{
	mGameWorld.Terminate();
}
void GameState::Update(const float deltaTime)
{
	mGameWorld.Update(deltaTime);
}
void GameState::Render()
{
	mGameWorld.Render();
}
void GameState::DebugUI()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		mGameWorld.DebugUI();
	ImGui::End();
}