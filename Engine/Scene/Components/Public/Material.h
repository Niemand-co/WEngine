#pragma once
#include "Scene/Components/Public/Component.h"

class RHIShader;

struct SurfaceData
{
	union
	{
		glm::vec4 buffer0;
		struct { glm::vec3 albedo; float roughness; };
	};

	union
	{
		glm::vec4 buffer1;
		struct { float mettalic, y, z, w; };
	};

};

class Material : public Component
{
	ENABLE_REFLECTION

public:

	Material(GameObject *pGameObject);

	~Material() = default;

	void SetShader(RHIShader* shader);

	RHIShader* GetShader();

	SurfaceData GetSurfaceData();

	void SetColor(glm::vec3 color);

	void SetRoughness(float roughness);

	void ShowInInspector();

private:

	glm::vec3 albedo;

	float roughness;

	float metallic;

	RHIShader* m_shader;
};


namespace WEngine
{
	namespace SRefl
	{
		template<>
		struct TypeInfo<Material> : public TypeInfoBase<Material, Component>
		{
			static constexpr std::string_view name = "Material";

			static constexpr FieldList list = 
			{
				Field("albedo", &Material::albedo),
				Field("roughness", &Material::roughness),
				Field("metallic", &Material::metallic),
				Field("m_shader", &Material::m_shader),
				Field("SetShader", &Material::SetShader),
				Field("GetShader", &Material::GetShader),
				Field("GetSurfaceData", &Material::GetSurfaceData),
				Field("SetColor", &Material::SetColor),
				Field("SetRoughness", &Material::SetRoughness),
				Field("ShowInInspector", &Material::ShowInInspector),
			};
		};
	}
}