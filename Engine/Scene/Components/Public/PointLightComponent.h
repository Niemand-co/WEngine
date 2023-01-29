#pragma once
#include "Scene/Components/Public/LightComponent.h"

class PointLightComponent : public LightComponent
{
public:

	enum { type = 9 };

	PointLightComponent();

	virtual ~PointLightComponent() = default;

};