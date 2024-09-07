#include "Precompiled.h"
#include "MeshComponent.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void MeshComponent::Deserialize(const rapidjson::Value& value)
{
	RenderObjectComponent::Deserialize(value);

	Model::MeshData& mesh = mModel.meshData.emplace_back();
	Model::MaterialData& material = mModel.materialData.emplace_back();
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			std::string shapeType = shapeData["Type"].GetString();
			if (shapeType == "Sphere")
			{
				uint32_t slices = static_cast<uint32_t>(shapeData["Slices"].GetInt());
				uint32_t rings = static_cast<uint32_t>(shapeData["Rings"].GetInt());
				float radius = shapeData["Radius"].GetFloat();
				mesh.mesh = MeshBuilder::CreateSphere(slices, rings, radius);
			}
			else if (shapeType == "GroundPlane")
			{
				uint32_t rows = static_cast<uint32_t>(shapeData["Rows"].GetInt());
				uint32_t columns = static_cast<uint32_t>(shapeData["Columns"].GetInt());
				float spacing = shapeData["Spacing"].GetFloat();
				mesh.mesh = MeshBuilder::CreateGroundPlane(rows, columns, spacing);
			}
			else if (shapeType == "SpriteQuad")
			{
				float width = shapeData["Width"].GetFloat();
				float height = shapeData["Height"].GetFloat();
				mesh.mesh = MeshBuilder::CreateSpriteQuad(width, height);
			}
			else
			{
				ASSERT(false, "MeshComponent: unrecognized shape type %s", shapeType.c_str());
			}
		}
		else
		{
			ASSERT(false, "MeshCompone: mush have a shape type");
		}
	}
	else
	{
		ASSERT(false, "MeshComponent: mush have shape data");
	}
	if (value.HasMember("Material"))
	{
		const auto& materialData = value["Material"].GetObj();
		if (materialData.HasMember("Ambient"))
		{
			const auto& color = materialData["Ambient"].GetArray();
			material.material.ambient.r = color[0].GetFloat();
			material.material.ambient.g = color[1].GetFloat();
			material.material.ambient.b = color[2].GetFloat();
			material.material.ambient.a = color[3].GetFloat();
		}
		if (materialData.HasMember("Diffuse"))
		{
			const auto& color = materialData["Ambient"].GetArray();
			material.material.diffuse.r = color[0].GetFloat();
			material.material.diffuse.g = color[1].GetFloat();
			material.material.diffuse.b = color[2].GetFloat();
			material.material.diffuse.a = color[3].GetFloat();
		}
		if (materialData.HasMember("Specular"))
		{
			const auto& color = materialData["Ambient"].GetArray();
			material.material.specular.r = color[0].GetFloat();
			material.material.specular.g = color[1].GetFloat();
			material.material.specular.b = color[2].GetFloat();
			material.material.specular.a = color[3].GetFloat();
		}
		if (materialData.HasMember("Emissive"))
		{
			const auto& color = materialData["Ambient"].GetArray();
			material.material.emissive.r = color[0].GetFloat();
			material.material.emissive.g = color[1].GetFloat();
			material.material.emissive.b = color[2].GetFloat();
			material.material.emissive.a = color[3].GetFloat();
		}
		if (materialData.HasMember("SpecularPower"))
		{
			material.material.power = materialData["SpecularPower"].GetFloat();
		}
	}
	if (value.HasMember("Textures"))
	{
		const auto& textureData = value["Textures"].GetObj();
		if (textureData.HasMember("DiffuseMap"))
		{
			material.diffuseMapName = textureData["DiffuseMap"].GetString();
		}
		if (textureData.HasMember("NormalMap"))
		{
			material.normalMapName = textureData["NormalMap"].GetString();
		}
		if (textureData.HasMember("BumpMap"))
		{
			material.bumpMapName = textureData["BumpMap"].GetString();
		}
		if (textureData.HasMember("SpecMap"))
		{
			material.specularMapName = textureData["SpecMap"].GetString();
		}
	}

}

const Model& MeshComponent::GetModel() const
{
	return mModel;
}
