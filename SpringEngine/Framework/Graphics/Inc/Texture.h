#pragma once

namespace SpringEngine::Graphics
{
	class Texture
	{
	public:
		static void UnbindPS(uint32_t slot);

		enum class Format
		{
			RGBA_U8,
			RGBA_U32
		};

		Texture() = default;
		virtual ~Texture();

		// delete texture copy
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		// allowing the move
		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;

		virtual void Initialize(const std::filesystem::path& fileName);
		virtual void Initialize(uint32_t width, uint32_t height, Format format);
		virtual void Terminate();

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const;

		void* GetRawData() const { return mShaderResourceView; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return mShaderResourceView; }
		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }

	protected:
		DXGI_FORMAT GetDXGIFormat(Format format);

		ID3D11ShaderResourceView* mShaderResourceView = nullptr;
		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
	};
}