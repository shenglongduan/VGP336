#pragma once

#include "Service.h"

namespace SpringEngine
{
	class Component;

	class UpdateService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Update);

		void Update(float deltaTime) override;

		void Register(Component* component);
		void UnRegister(Component* component);

	private:

		using UpdateComponents = std::vector<Component*>;
		UpdateComponents mUpdateComponents;
		UpdateComponents mPendingComponents;
		
		bool mIsUpdating = false;

	};
}