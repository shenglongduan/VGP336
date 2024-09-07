#pragma once

#include "Model.h"

namespace SpringEngine::Graphics
{
	using ModelId = std::size_t;
	class ModelManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();

		ModelManager() = default;
		~ModelManager() = default;

		ModelManager(const ModelManager&) = delete;
		ModelManager(const ModelManager&&) = delete;
		ModelManager& operator=(const ModelManager&) = delete;
		ModelManager& operator=(const ModelManager&&) = delete;

		ModelId GetModelId(const std::filesystem::path& filepath);
		ModelId LoadModel(const std::filesystem::path& filepath);
		void AddAnimation(ModelId id, const std::filesystem::path& filepath);
		const Model* GetModel(ModelId id);

	private:
		using ModelCache = std::map<ModelId, std::unique_ptr<Model>>;
		ModelCache mInventory;
	};
}