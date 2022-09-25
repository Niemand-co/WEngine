#pragma once
#ifndef SR_COMPONENT_H
#define SR_COMPONENT_H
#include "Scene/Public/Object.h"

class Component : public Object
{
public:
	enum class ComponentType
	{
		Transformer = 0,
		Camera,
		MeshFilter,
	};

public:
	virtual ~Component() = default;

	virtual ComponentType GetType() { return m_type; }
	virtual bool IsType(ComponentType type) { return type == m_type; }

protected:
	ComponentType m_type;
};

#endif