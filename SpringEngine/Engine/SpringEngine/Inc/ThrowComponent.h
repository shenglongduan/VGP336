#pragma once

#include "Component.h"


namespace SpringEngine
{
	class ThrowComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Throw);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void Deserialize(const rapidjson::Value& value) override;

	private:
		int mStartPlayIndex = -1;
		int mThrowFrame = -1;
		bool mLock = false;
		GameObject* mThrowObject = nullptr;
		Math::Vector3 mThrowVelocity = {0.0f, 0.0f, 0.0f};
		Math::Vector3 mThrowPosition = {0.0f, 0.0f, 0.0f};
		Graphics::Animator* mAnimator = nullptr;
	};
}