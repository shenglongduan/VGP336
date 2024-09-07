#include"GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{

	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<UpdateService>();
	mGameWorld.Initialize();

	mGameWorld.AddGameObject(&mGameObject);

	mGameObject.AddComponent<TransformComponent>();
	CameraComponent* cc = mGameObject.AddComponent<CameraComponent>();
	mGameObject.AddComponent<FPSCameraComponent>();
	mGameObject.Initialize();
	std::string name = "GameObject1";
	mGameObject.SetName(name);


	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(cc->GetCamera());
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

}
void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mGameObject.Terminate();

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