#pragma once

#include "Component.h"

namespace SpringEngine
{
	class AnimatorComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Animator);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;
		void Deserialize(const rapidjson::Value& value) override;

		bool Play(int index, bool looping = false);
		Graphics::Animator& GetAnimator();
		const Graphics::Animator& GetAnimator() const;

		void AddKeyFrame(int frame, std::function<void()> callback);
		int GetStartPlayIndex();
	private:
		int mStartPlayIndex = -1;
		Graphics::Animator mAnimator;
		bool mIdle = false;
	};
}