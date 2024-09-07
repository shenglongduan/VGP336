#include"GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());


	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);


	ModelId modelId = ModelManager::Get()->LoadModel(L"../../Assets/Models/Character_1/Character01.model");
	mGoblin = CreateRenderGroup(modelId);


	modelId = ModelManager::Get()->LoadModel(L"../../Assets/Models/Character_3/Character03.model");
	mVanguard = CreateRenderGroup(modelId);

	mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_200x200.raw", 10.0f);

	mGround.meshBuffer.Initialize(mTerrain.GetMesh());
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_texture.jpg");
	mGround.normalMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_normal.jpg");
	mGround.specMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_spec.jpg");
	mGround.specMapId = TextureManager::Get()->LoadTexture(L"terrain/grass_2048.jpg");

	Math::Vector3 spawnPos = Vector3::Zero;
	mVangaurdPosition.x = 10.0f;
	mVangaurdPosition.z = 10.0f;
	mVangaurdPosition.y = mTerrain.GetHeight(mVangaurdPosition);
	SetRenderGroupPosition(mVanguard, mVangaurdPosition);

	mGoblinPosition.x = 5.0f;
	mGoblinPosition.z = 15.0f;
	mGoblinPosition.y = mTerrain.GetHeight(mGoblinPosition);
	SetRenderGroupPosition(mGoblin, mGoblinPosition);
}
void GameState::Terminate()
{
	mGround.Terminate();
	CleanupRenderGroup(mVanguard);
	CleanupRenderGroup(mGoblin);
	mStandardEffect.Terminate();
	mTerrainEffect.Terminate();
	mStandardEffect.Terminate();

}
void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);
}
void GameState::Render()
{
	mShadowEffect.Begin();
	DrawRenderGroup(mShadowEffect, mVanguard);
	DrawRenderGroup(mShadowEffect, mGoblin);
	mShadowEffect.End();

	mTerrainEffect.Begin();
	mTerrainEffect.Render(mGround);
	mTerrainEffect.End();


	mStandardEffect.Begin();
	DrawRenderGroup(mStandardEffect, mVanguard);
	DrawRenderGroup(mStandardEffect, mGoblin);
	mStandardEffect.End();

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
	float terrainWidth = mTerrain.GetWidth();
	float terrainHeight = mTerrain.GetHeight();
	if (ImGui::CollapsingHeader("VangaurdPosition", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("PosX##Vanguard", &mVangaurdPosition.x, 0.1f, 0.0f, terrainWidth))
		{
			mVangaurdPosition.y = mTerrain.GetHeight(mVangaurdPosition);
			SetRenderGroupPosition(mVanguard, mVangaurdPosition);
		}
		if (ImGui::DragFloat("PosZ##Vanguard", &mVangaurdPosition.z, 0.1f, 0.0f, terrainHeight))
		{
			mVangaurdPosition.y = mTerrain.GetHeight(mVangaurdPosition);
			SetRenderGroupPosition(mVanguard, mVangaurdPosition);
		}
	}
	if (ImGui::CollapsingHeader("GoblinPosition", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("PosX##Goblin", &mGoblinPosition.x, 0.1f, 0.0f, terrainWidth))
		{
			mGoblinPosition.y = mTerrain.GetHeight(mGoblinPosition);
			SetRenderGroupPosition(mGoblin, mGoblinPosition);
		}
		if (ImGui::DragFloat("PosZ##Goblin", &mGoblinPosition.z, 0.1f, 0.0f, terrainHeight))
		{
			mGoblinPosition.y = mTerrain.GetHeight(mGoblinPosition);
			SetRenderGroupPosition(mGoblin, mGoblinPosition);
		}
	}

	mStandardEffect.DebugUI();
	mTerrainEffect.DebugUI();
	mShadowEffect.DebugUI();
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
	float characterHeight = 3.0f;
Math:Vector3 cameraPos = mCamera.GetPosition();
	cameraPos.y = mTerrain.GetHeight(cameraPos) + characterHeight;
	mCamera.SetPosition(cameraPos);
}