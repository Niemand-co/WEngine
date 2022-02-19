#pragma once
#ifndef SR_LIGHT_H
#define SR_LIGHT_H

#include "Scene/Entity.h"
#include "Math/Vector.h"

class Light : public Entity
{
public:
	virtual ~Light() = default;
	Vec3 GetColor() { return m_color; }
	void SetColor(const Vec3& color) { m_color = color; }
private:
	Vec3 m_color;
};

class ParallelLight : public Light
{
public:
	ParallelLight();
	~ParallelLight();
private:
	
};

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();
	Vec3 GetPos() { return m_position; }
	void SetPos(const Vec3& pos) { m_position = pos; }
private:
	Vec3 m_position;
};

#endif