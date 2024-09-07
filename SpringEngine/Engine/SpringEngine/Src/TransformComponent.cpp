#include "Precompiled.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include "SaveUtil.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void TransformComponent::DebugUI()
{
	SimpleDraw::AddTransform(GetMatrix4());

	std::string idTag = GetOwner().GetName();
	std::string imguiName = "Position##" + idTag;
	ImGui::DragFloat3(imguiName.c_str(), &position.x);
	imguiName = "Rotation##" + idTag;
	ImGui::DragFloat3(imguiName.c_str(), &rotation.x, 0.01f);
	imguiName = "Scale##" + idTag;
	ImGui::DragFloat3(imguiName.c_str(), &scale.x, 0.01f);
}

void TransformComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Position", position, doc, componentValue);
	SaveUtil::SaveQuaternion("Rotation", rotation, doc, componentValue);
	SaveUtil::SaveVector3("Scale", scale, doc, componentValue);
	value.AddMember("TransformComponent", componentValue, doc.GetAllocator());
}

void SpringEngine::TransformComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		const auto& pos = value["Position"].GetArray();
		position.x = pos[0].GetFloat();
		position.y = pos[1].GetFloat();
		position.z = pos[2].GetFloat();
	}
	if (value.HasMember("Rotation"))
	{
		const auto& rot = value["Rotation"].GetArray();
		rotation.x = rot[0].GetFloat();
		rotation.y = rot[1].GetFloat();
		rotation.z = rot[2].GetFloat();
		rotation.w = rot[3].GetFloat();
	}
	if (value.HasMember("Scale"))
	{
		const auto& s = value["Scale"].GetArray();
		scale.x = s[0].GetFloat();
		scale.y = s[1].GetFloat();
		scale.z = s[2].GetFloat();
	}
}
