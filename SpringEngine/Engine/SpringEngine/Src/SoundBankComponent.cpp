#include "Precompiled.h"
#include "SoundBankComponent.h"

#include "GameObject.h"

using namespace SpringEngine;
using namespace SpringEngine::Audio;


void SoundBankComPent::Initialize()
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	for (auto& date : mSoundEffects)
	{
		date.second.soundId = sm->Load(date.second.fileName);
	}
}

void SoundBankComPent::Terminate()
{
}

void SoundBankComPent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("SoundEffects"))
	{
		auto soundEffects = value["SoundEffects"].GetObj();
		for (auto& effect : soundEffects)
		{
			SoundEffectDate& date = mSoundEffects[effect.name.GetString()];
			if (effect.value.HasMember("FileName"))
			{
				date.fileName = effect.value["FileName"].GetString();
			}
			if (effect.value.HasMember("Looping"))
			{
				date.isLoopind = effect.value["Looping"].GetBool();
			}
		}
	}
}

void SoundBankComPent::DebugUI()
{
	std::string playButtonTag = "PlaySound##" + GetOwner().GetName();
	std::string stopButtonTag = "StopSound##" + GetOwner().GetName();
	std::string soundEffectsHeader = "SoundEffects##" + GetOwner().GetName();
	ImGui::Indent();
	if (ImGui::CollapsingHeader(soundEffectsHeader.c_str()))
	{
		for (auto& date : mSoundEffects)
		{
			std::string buttonName = date.first + playButtonTag;
			if (ImGui::Button(buttonName.c_str()))
			{
				Play(date.first);
			}
			ImGui::SameLine();
			buttonName = date.first + stopButtonTag;
			if (ImGui::Button(buttonName.c_str()))
			{
				Stop(date.first);
			}
		}
	}
	ImGui::Unindent();
}

void SoundBankComPent::Play(const std::string& key)
{
	auto data = mSoundEffects.find(key);
	if (data != mSoundEffects.end())
	{
		SoundEffectManager::Get()->Stop(data->second.soundId);
		SoundEffectManager::Get()->Play(data->second.soundId);
	}
}

void SoundBankComPent::Stop(const std::string& key)
{
	auto data = mSoundEffects.find(key);
	if (data != mSoundEffects.end())
	{
		SoundEffectManager::Get()->Stop(data->second.soundId);
	}
}

void SoundBankComPent::StopAll()
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	for (auto data : mSoundEffects)
	{
		sm->Stop(data.second.soundId);
	}
}
