#pragma once
#include <SpringEngine/Inc/SpringEngine.h>

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

class GameState : public SpringEngine::AppState
{
public:
	enum class ShapeType
	{
		Transform,
		Sphere,
		AABB,
		AABBFilled,
		Custom,
		Count
	};
	virtual ~GameState() = default;

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCameraControl(float deltaTime);

	Camera mCamera;
	ShapeType mShapeType = ShapeType::Transform;
	float mSphereRadius = 1.0f;
	Color mShapeColor = Colors::BlueViolet;
};