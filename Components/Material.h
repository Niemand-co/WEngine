#pragma once
#ifndef SR_MATERIAL_H
#define SR_MATERIAL_H

#include "Component.h"
#include "Math/Vector.h"
#include "Utils/Shader.h"

class Material : public Component
{
public:
	Material();
	~Material() = default;

	void SetShader(Shader* shader);
	Shader* GetShader();
private:
	Vec3 color;
	float roughness;
	Shader* m_shader;
};

#endif