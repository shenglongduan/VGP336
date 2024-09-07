#pragma once


#include"Colors.h"

namespace SpringEngine::Graphics
{
	struct DirectionalLight
	{
		Math::Vector3 direction = Math::Vector3::Zero;
		float padding = 0.0f;
		Color ambient = Colors::White;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
	};
	struct PointLight
	{
		Math::Vector3 direction = Math::Vector3::Zero;
		float padding0 = 0.0f;
		Math::Vector3 Position = Math::Vector3::Zero;
		float padding1 = 0.0f;
		Color ambient = Colors::White;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
		float attenConstant = -1.0f;
		float attenLinear = 0.0f;
		float attenQuad = 0.0f;
		float padding2 = 0.0f;
	};

}