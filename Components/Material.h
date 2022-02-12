#pragma once
#ifndef SR_MATERIAL_H
#define SR_MATERIAL_H

#include "Component.h"
#include "Math/Vector.h"

class Material : public Component
{
public:
	Material();
	~Material() = default;

private:
	Vec3 color;
	float roughness;
};

#endif