#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace SpringEngine::Graphics
{
	class RenderObject;
	class Texture;

	class PostProcessingEffect
	{
	public:
		enum class Mode
		{
			None,
			Monochrome,
			Invert,
			Mirror,
			Blur,
			Combine2,
			MotionBlur,
			ChromaticAberration
		};

		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetTexture(const Texture* texture, uint32_t slot = 0);
		void SetMode(Mode mode);

		void Debug();
		
	private:
		struct PostProcessingData
		{
			int mode = 0;
			float params0 = 0.0f;
			float params1 = 0.0f;
			float params2 = 0.0f;
		};

		using PostProcessBuffer = TypedConstantBuffer<PostProcessingData>;
		PostProcessBuffer mPostProcessBuffer;

		VertexShader mVertecShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		std::array<const Texture*, 4> mTextures;
		Mode mMode = Mode::None;

		float mMirrorScaleX = -1.0f;
		float mMirrorScaleY = -1.0f;
		float mBlurStrenth = 5.0f;
	};
}