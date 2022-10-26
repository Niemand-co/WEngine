#pragma once
#include "Scene/Public/Object.h"

enum class Direction
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class RotateDirection
{
	Pitch = 0,
	Yaw,
	Roll
};

class Component : public Object
{
public:
	enum class ComponentType
	{
		Transformer = 0,
		Camera,
		MeshFilter,
		Material,
	};

public:

	virtual ~Component() = default;

	virtual ComponentType GetType() { return m_type; }

	virtual bool IsType(ComponentType type) 
	{
		return type == m_type; 
	}

protected:

	ComponentType m_type;

};