#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace SpringEngine::Graphics
{
	class Camera;
	class RenderObject;

	class SimpleEffect
	{
	public:
		void Initialize();
		void Terminate();
		void Begin();
		void End();
		void Render(const RenderObject& renderObject);
		void SetCamera(const Camera& camera);

	private:
		const Camera* mCamera = nullptr;

		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
	};
}