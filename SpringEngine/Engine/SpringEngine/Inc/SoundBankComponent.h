#pragma once

#include "Component.h"

namespace SpringEngine
{
	class SoundBankComPent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::SoundBank);

		void Initialize() override;
		void Terminate() override;
		void Deserialize(const rapidjson::Value& value) override;

		void DebugUI() override;

		void Play(const std::string& key);
		void Stop(const std::string& key);
		void StopAll();
	private:
		struct SoundEffectDate
		{
			std::string fileName;
			Audio::SoundId soundId;
			bool isLoopind = false;
		};

		using SoundEffects = std::unordered_map<std::string, SoundEffectDate>;
		SoundEffects mSoundEffects;
	};
}