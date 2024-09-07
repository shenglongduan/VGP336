#include"GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{

	CameraService* cs =  mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<UpdateService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize();

	mGameWorld.AddGameObject(&mCameraObject);
	mGameWorld.AddGameObject(&mTestGameObject);

	GameObjectFactory::Make("../../Assets/Templates/fps_camera.json", mCameraObject);
	std::string name = "CameraObject";
	mCameraObject.SetName(name);
	
	//Game Object Factory
	GameObjectFactory::Make("../../Assets/Templates/test_model.json", mTestGameObject);
	name = "test_object";
	mTestGameObject.SetName(name);

}
void GameState::Terminate()
{
	mCameraObject.Terminate();
	mTestGameObject.Terminate();

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