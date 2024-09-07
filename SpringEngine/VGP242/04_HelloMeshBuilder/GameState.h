#pragma once
#include <SpringEngine/Inc/SpringEngine.h>

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

class GameState : public SpringEngine::AppState
{
public:
	virtual ~GameState() = default;

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	Camera mCamera;
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMeshBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;

	Texture mDiffuseTexture;
	Sampler mSampler;
};