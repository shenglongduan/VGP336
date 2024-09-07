#pragma once


namespace SpringEngine
{
	class GameObject;
	class Component;

	using CustomMake = std::function<Component*(const std::string&, GameObject&)>;

	namespace GameObjectFactory
	{ 
		void SetCustomMake(CustomMake custommake);
		void Make(const std::filesystem::path& templatePath, GameObject& gameObject);
	}
}