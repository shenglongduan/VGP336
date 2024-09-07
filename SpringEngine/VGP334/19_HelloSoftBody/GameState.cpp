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


	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	int rows = 10;
	int cols = 10;
	mClothMesh = MeshBuilder::CreateGroundPlane(rows, cols, 1.0f);
	for (auto& v : mClothMesh.vertices)
	{
		v.position.y = 10.0f;
	}

	int lastVertex = mClothMesh.vertices.size() - 1;
	int lastVertexOppSide = lastVertex - rows;
	mClothSB.Initialize(mClothMesh, 1.0f, { lastVertex, lastVertexOppSide });
		mCloth.meshBuffer.Initialize(nullptr, sizeof(Vertex), mClothMesh.vertices.size(), mClothMesh.indices.data(), mClothMesh.indices.size());
	mCloth.diffuseMapId = TextureManager::Get()->LoadTexture("sun.jpg");

}
void GameState::Terminate()
{
	mCloth.Terminate();
	mClothSB.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);
}
void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	mCloth.meshBuffer.Update(mClothMesh.vertices.data(), mClothMesh.vertices.size());
	mStandardEffect.Begin();
		mStandardEffect.Render(mCloth);
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
	mStandardEffect.DebugUI();
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