#pragma once
#include "Scene/Components/Public/LightComponent.h"

class SpotLightComponent : public LightComponent
{
public:

	enum { type = 10 };

	SpotLightComponent();

	virtual ~SpotLightComponent() = default;

};