#pragma once

#include "Component.h"
#include "GameObjectHandle.h"

namespace SpringEngine
{
	class GameWorld;

	class GameObject final
	{
	public:
		GameObject() = default;

		void Initialize();
		void Terminate();
		void DebugUI();

		void SetName(std::string& name);
		const std::string& GetName() const;
		uint32_t GetUniqueId() const;

		GameWorld& GetWorld();
		const GameWorld& GetWorld() const;
		const GameObjectHandle& GetHandle() const;
		
		template <class ComponentType>
		ComponentType* AddComponent()
		{
			static_assert(std::is_base_of_v<Component,ComponentType>,
				"GameObject: ComponentType Must be of type Component");

				ASSERT(!mInitialized, "GameObject: cannot add components once initialized");
				ASSERT(!HasA<ComponentType>(), "GameObject: already has components type");
				ASSERT(ComponentType::StaticGetTypeId() != static_cast<uint32_t>(ComponentId::Invalid), "GameObject: invalid component ID");

				auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
				newComponent->mOwner = this;
				return static_cast<ComponentType*>(newComponent.get());
		}

		template <class ComponentType>
		bool HasA()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>,
				"GameObject: ComponentType Must be of type Component");
				for (auto& component : mComponents)
				{
					if (component->GetTypeId() == ComponentType::StaticGetTypeId())
					{
						return true;
					}
				}
				return false;
		}

		template <class ComponentType>
		const ComponentType* GetComponent() const
		{
			static_assert(std::is_base_of_v<Component, ComponentType>,
				"GameObject: ComponentType Must be of type Component");
				for (auto& component : mComponents)
				{
					if (component->GetTypeId() == ComponentType::StaticGetTypeId())
					{
						return static_cast<ComponentType*>(component.get());

					}
				}
				return nullptr;
		}

		template <class ComponentType>
		ComponentType* GetComponent()
		{
			const GameObject* thisConst = static_cast<const GameObject*>(this);
			return const_cast<ComponentType*>(thisConst->GetComponent<ComponentType>());
		}

	private:
		std::string mName = "EMPTY";
		bool mInitialized = false;
		uint32_t mUniqueId = 0;

		using Components = std::vector<std::unique_ptr<Component>>;
		Components mComponents;

		friend class GameWorld;
		GameWorld* mWorld = nullptr;
		GameObjectHandle mHandle;
		std::string mTemplateFile;
	};
}