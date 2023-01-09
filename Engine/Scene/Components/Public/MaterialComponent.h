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

enum Property
{
	Float = 0,
	Range,
	Color,
	Shader,
};

class MaterialComponent : public Component
{
	ENABLE_REFLECTION

	friend class VulkanGui;

	friend class GameObject;

public:

	typedef MaterialComponent type;

	MaterialComponent(GameObject *pGameObject);

	~MaterialComponent() = default;

	virtual void ShowInInspector() override;

	void SetShader(RHIShader* shader);

	RHIShader* GetShader();

	SurfaceData GetSurfaceData();

	void SetColor(glm::vec4 color);

	void SetRoughness(float roughness);

private:

	[[Color(1, 1, 1, 1)]]
	glm::vec4 albedo;

	[[Range(0.01, 1)]]
	float roughness;

	[[Range(0, 1)]]
	float metallic;

	RHIShader* m_shader;
};


//namespace WEngine
//{
//	namespace SRefl
//	{
//		template<>
//		struct TypeInfo<Material> : public TypeInfoBase<Material, Component>
//		{
//			static constexpr WString name = "Material";
//
//			static constexpr FieldList fields = 
//			{
//				Field("albedo", &Material::albedo, AttrList{ Color, 1, 1, 1, 1 }),
//				Field("roughness", &Material::roughness, AttrList{ Range,  0.01f, 1.0f }),
//				Field("metallic", &Material::metallic, AttrList{ Range, 0.0f, 1.0f }),
//				Field("m_shader", &Material::m_shader, AttrList{ Shader }),
//				Field("SetShader", &Material::SetShader, AttrList{}),
//				Field("GetShader", &Material::GetShader, AttrList{}),
//				Field("GetSurfaceData", &Material::GetSurfaceData, AttrList{}),
//				Field("SetColor", &Material::SetColor, AttrList{}),
//				Field("SetRoughness", &Material::SetRoughness, AttrList{}),
//				Field("ShowInInspector", &Material::ShowInInspector, AttrList{}),
//			};
//		};
//	}
//}