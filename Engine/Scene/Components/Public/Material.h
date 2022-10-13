#pragma once
#include "Scene/Components/Public/Component.h"

class RHIShader;

struct SurfaceData
{
	glm::vec3 albedo;

	float roughness;

	float metallic;
};

class Material : public Component
{
public:

	Material();

	~Material() = default;

	void SetShader(RHIShader* shader);

	RHIShader* GetShader();

	SurfaceData GetSurfaceData();

private:

	glm::vec3 albedo;

	float roughness;

	float metallic;

	RHIShader* m_shader;
};
