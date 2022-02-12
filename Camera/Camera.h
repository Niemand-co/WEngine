#pragma once
#ifndef SR_CAMERA_H
#define SR_CAMERA_H

#include "Math/Matrix.h"
#include "Scene/Entity.h"

class Camera : public Entity
{
public:
	enum class direction
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	Camera();
	Camera(Vec3 position, Vec3 front);
	~Camera() = default;

	void Move(direction dir, float dis);

	Matrix4x4f GetViewMatrix();
private:
	void UpdateViewMatrix();
private:
	Vec3 m_pos;
	Vec3 m_front;
	Matrix4x4f m_viewMatrix;
};

#endif