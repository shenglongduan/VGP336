#include"GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-6.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);

	ParticleSystemInfo info;
	info.spawnPosition = Math::Vector3::Zero;
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnDelay = 0.0f;
	info.systemLifeTime = 99999.0f;
	info.minParticlePerEmit = 1;
	info.maxParticlePerEmit = 3;
	info.minTimeBetweenEmit = 1.0f;
	info.maxTimeBetweenEmit = 3.0f;
	info.minSpawnAngle = -10.0f * 3.141592 / 180.0f;
	info.maxSpawnAngle = 10.0f * 3.141592 / 180.0f;
	info.minSpeed = 5.0f;
	info.maxSpeed = 10.0f;
	info.minLifeTime = 0.3f;
	info.maxLifeTime = 1.0f;
	info.minStartColor = Colors::White;
	info.maxStartColor = Colors::White;
	info.minEndColor = Colors::White;
	info.maxEndColor = Colors::White;
	info.minStartScale = Math::Vector3::One;
	info.maxStartScale = Math::Vector3::One;
	info.minEndScale = Math::Vector3::Zero;
	info.maxEndScale = Math::Vector3::Zero;
	info.maxParticles = 100;
	info.particleTextureId = TextureManager::Get()->LoadTexture("sun.jpg");
	mParticleSystem.Initialize(info);
	mParticleSystem.SetCamera(mCamera);
}
void GameState::Terminate()
{
	mParticleSystem.Terminate();
	mParticleEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	mParticleSystem.Update(deltaTime);
	UpdateCameraControl(deltaTime);
}
void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);
		mParticleEffect.Begin();
		mParticleSystem.Render(mParticleEffect);
	mParticleEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.1f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	mParticleSystem.DebugUI();
	Physics::PhysicsWorld::Get()->DebugUI();
	ImGui::End();
}
void GameState::UpdateCameraControl(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}