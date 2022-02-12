#include "Camera.h"

Camera::Camera()
	: m_pos(Vec3()), m_front(Vec3(0.0f, 0.0f, -1.0f))
{
	UpdateViewMatrix();
	SetVisible(false);
}

Camera::Camera(Vec3 position, Vec3 front = Vec3(0.0f, 0.0f, -1.0f))
	: m_pos(position), m_front(front)
{
	UpdateViewMatrix();
	SetVisible(false);
}

void Camera::Move(direction dir, float dis)
{
	switch (dir)
	{
		case direction::FORWARD:
		{
			m_pos += dis * Vec3(0.0f, 0.0f, -1.0f);
			break;
		}
		case direction::BACKWARD:
		{
			m_pos += dis * Vec3(0.0f, 0.0f, 1.0f);
			break;
		}
		case direction::LEFT:
		{
			m_pos += dis * Vec3(-1.0f, 0.0f, 0.0f);
			break;
		}
		case direction::RIGHT:
		{
			m_pos += dis * Vec3(1.0f, 0.0f, 0.0f);
			break;
		}
		case direction::UP:
		{
			m_pos += dis * Vec3(0.0f, 1.0f, 0.0f);
			break;
		}
		case direction::DOWN:
		{
			m_pos += dis * Vec3(0.0f, -1.0f, 0.0f);
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
	LookAt(m_viewMatrix, m_pos, m_pos + m_front, Vec3(0.0f, 1.0f, 0.0f));
}