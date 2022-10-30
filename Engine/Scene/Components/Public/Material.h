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
