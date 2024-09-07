#include "GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	// create shapes in NDC normalized device coordinate
	// -1 to 1 (x, y, z)
	MeshPC mesh;
	//mesh.vertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Blue });
	//mesh.vertices.push_back({ {0.0f, 0.75f, 0.0f}, Colors::Red });
	//mesh.vertices.push_back({ {0.5f, 0.0f, 0.0f}, Colors::Green });

	//mesh.vertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Blue });
	//mesh.vertices.push_back({ {-0.5f, 0.75f, 0.0f}, Colors::Blue });
	//mesh.vertices.push_back({ {0.0f, 0.75f, 0.0f}, Colors::Red });

	//mesh.vertices.push_back({ {0.0f, 0.75f, 0.0f}, Colors::Red });
	//mesh.vertices.push_back({ {0.5f, 0.75f, 0.0f}, Colors::Blue });
	//mesh.vertices.push_back({ {0.5f, 0.0f, 0.0f}, Colors::Green });

	mesh.vertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Blue });
	mesh.vertices.push_back({ {-0.5f, 0.75f, 0.0f}, Colors::Blue });
	mesh.vertices.push_back({ {0.0f, 0.75f, 0.0f}, Colors::Red });
	mesh.vertices.push_back({ {0.5f, 0.75f, 0.0f}, Colors::Blue });
	mesh.vertices.push_back({ {0.5f, 0.0f, 0.0f}, Colors::Green });

	mesh.indices = {
		0, 2, 4,
		0, 1, 2,
		2, 3, 4
	};

	auto device = GraphicsSystem::Get()->GetDevice();
	// Create a vertex buffer
	mMeshBuffer.Initialize(mesh);

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";

	// Create a vertex shader
	mVertexShader.Initialize<VertexPC>(shaderFile);

	// Create a pixel shader
	mPixelShader.Initialize(shaderFile);

	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	mVertices.clear();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

float totalTime = 0.0f;
Vector3 position(0.0f);
void GameState::Update(float deltaTime)
{
	totalTime += deltaTime;
	position.y = std::sin(totalTime);
}

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	Matrix4 matWorld = Matrix4::Translation(position);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;
	Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMeshBuffer.Render();
}
