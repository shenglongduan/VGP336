#pragma once

#include "Component.h"

namespace SpringEngine
{
	class CameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Camera);

		void Initialize() override;
		void Terminate() override;
		virtual void Deserialize(const rapidjson::Value& value) override;

		Graphics::Camera& GetCamera();
		const Graphics::Camera& GetCamera() const;

	private:
		Graphics::Camera mCamera;
	};
}