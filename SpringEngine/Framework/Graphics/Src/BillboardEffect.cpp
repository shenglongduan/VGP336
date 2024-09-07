#include "Precompiled.h"
//#include "BillboardEffect.h"
//#include "Camera.h"
//#include "GraphicsSystem.h"
//#include "RenderObject.h"
//#include "TextureManager.h"
//#include "VertexTypes.h"
//
//using namespace SpringEngine;
//using namespace SpringEngine::Graphics;
//
//void SpringEngine::Graphic::BillboardEffect::Initialize()
//{
//	std::filesystem::path filePath = L"../../Assets/Shaders/DoBillboard.fx";
//	mVertexShader.Initialize<Vertex>(filePath);
//	mPixelShader.Initialize(filePath);
//	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
//	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
//}
//
//void SpringEngine::Graphic::BillboardEffect::Terminate()
//{
//	mSampler.Terminate();
//	mConstantBuffer.Terminate();
//	mPixelShader.Terminate();
//	mVertexShader.Terminate();
//}
//
//void SpringEngine::Graphic::BillboardEffect::Begin()
//{
//	mSampler.BindPS(0);
//	mConstantBuffer.BindVS(0);
//	mPixelShader.Bind();
//	mVertexShader.Bind();
//}
//
//void SpringEngine::Graphic::BillboardEffect::End()
//{
//	Texture::UnbindPS(0);
//}
//
//void SpringEngine::Graphic::BillboardEffect::Render(const RenderObject& renderObject)
//{
//	//Math::Matrix4 matWorld = Math::Matrix4::Translation(renderObject.transform.position);
//	Math::Matrix4 matView = mCamera->GerViewMatrix();
//}
//
//void SpringEngine::Graphic::BillboardEffect::SetCamera(const Camera& camera)
//{
//	mCamera = &camera;
//}