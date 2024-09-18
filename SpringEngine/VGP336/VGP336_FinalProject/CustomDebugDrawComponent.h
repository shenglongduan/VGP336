#pragma once

#include <SpringEngine/Inc/SpringEngine.h>
#include "TypeIds.h"
class CustomDebugDrawComponent :public SpringEngine::Component
{
public:
	SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;

	void AddDebugDraw();
	void Deserialize(const rapidjson::Value& value) override;

private:
	SpringEngine::TransformComponent* mTransformComponent = nullptr;
	SpringEngine::Math::Vector3 mPosition = SpringEngine::Math::Vector3::Zero;
	SpringEngine::Math::Vector3 mVelocity = SpringEngine::Math::Vector3::Zero;
	SpringEngine::Color mColor = SpringEngine::Colors::White;
	uint32_t mSlices = 0;
	uint32_t mRings = 0;
	float mRadius = 0.0f;

};