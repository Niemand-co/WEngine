#include "pch.h"
#include "Scene/Components/Public/Camera.h"

Camera::Camera()
	: m_pos(Vector3()), m_front(Vector3(0.0f, 0.0f, -1.0f))
{
	UpdateViewMatrix();
}

Camera::Camera(Vector3 position, Vector3 front = Vector3(0.0f, 0.0f, -1.0f))
	: m_pos(position), m_front(front)
{
	UpdateViewMatrix();
}

void Camera::Move(direction dir, float dis)
{
	switch (dir)
	{
		case direction::FORWARD:
		{
			m_pos += dis * Vector3(0.0f, 0.0f, -1.0f);
			break;
		}
		case direction::BACKWARD:
		{
			m_pos += dis * Vector3(0.0f, 0.0f, 1.0f);
			break;
		}
		case direction::LEFT:
		{
			m_pos += dis * Vector3(-1.0f, 0.0f, 0.0f);
			break;
		}
		case direction::RIGHT:
		{
			m_pos += dis * Vector3(1.0f, 0.0f, 0.0f);
			break;
		}
		case direction::UP:
		{
			m_pos += dis * Vector3(0.0f, 1.0f, 0.0f);
			break;
		}
		case direction::DOWN:
		{
			m_pos += dis * Vector3(0.0f, -1.0f, 0.0f);
		}
	}
	UpdateViewMatrix();
}

Matrix4x4f Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

void Camera::UpdateViewMatrix()
{
	m_viewMatrix = Matrix4x4f::GetIdentityMatrix();
	LookAt(m_viewMatrix, m_pos, Vector3(), Vector3(0.0f, 1.0f, 0.0f));
}