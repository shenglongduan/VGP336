#pragma once
#include <SpringEngine/Inc/SpringEngine.h>

class GameState : public SpringEngine::AppState
{
public:
	virtual ~GameState() = default;

	void Initialize() override;
	void Terminate() override;
	void Render() override;

protected:
	virtual void CreateShape() {}
	struct Vertex
	{
		SpringEngine::Math::Vector3 position;
		SpringEngine::Color color;
	};

	using Vertices = std::vector<Vertex>;
	Vertices mVertices;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};

class Triangle : public GameState
{
public:
	void Update(float deltaTime) override;
protected:
	void CreateShape() override;
};

class Square : public GameState
{
public:
	void Update(float deltaTime) override;
protected:
	void CreateShape() override;
};