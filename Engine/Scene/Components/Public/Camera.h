#pragma once
#ifndef SR_CAMERA_H
#define SR_CAMERA_H

#include "Scene/Components/Public/Component.h"

class Camera : public Component
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

	Camera(Vector3 position, Vector3 front);

	~Camera() = default;

	void Move(direction dir, float dis);

	Vector3 GetPos(){return m_pos;}

	Matrix4x4f GetViewMatrix();

private:

	void UpdateViewMatrix();

private:

	Vector3 m_pos;

	Vector3 m_front;

	Matrix4x4f m_viewMatrix;
};

#endif