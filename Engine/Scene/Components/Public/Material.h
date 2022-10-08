#pragma once
#include "Scene/Components/Public/Component.h"

class RHIShader;

class Material : public Component
{
public:

	Material();

	~Material() = default;

	void SetShader(RHIShader* shader);

	RHIShader* GetShader();

private:

	Vector3 color;

	float roughness;

	RHIShader* m_shader;
};
