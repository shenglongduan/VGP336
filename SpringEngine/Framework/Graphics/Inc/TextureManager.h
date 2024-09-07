#pragma once

#include "Texture.h"

namespace SpringEngine::Graphics
{
	using TextureId = std::size_t;

	class TextureManager
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static TextureManager* Get();

		TextureManager() = default;
		~TextureManager();
		
		TextureManager(const TextureManager&) = delete;
		TextureManager(const TextureManager&&) = delete;
		TextureManager& operator= (const TextureManager&) = delete;
		TextureManager& operator= (const TextureManager&&) = delete;

		void SetRootDirectiory(const std::filesystem::path& root);

		TextureId LoadTexture(const std::filesystem::path& fileName, bool useRootDir = true);
		const Texture* GetTexture(TextureId id);

		void BindVS(TextureId id, uint32_t slot) const;
		void BindPS(TextureId id, uint32_t slot) const;

	private:
		using Inventory = std::unordered_map<TextureId, std::unique_ptr<Texture>>;
		Inventory mInventory;

		std::filesystem::path mRootDirectory;
	};
}