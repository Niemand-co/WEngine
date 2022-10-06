#include "pch.h"
#include "Scene/Components/Public/Camera.h"
#include "Render/Public/ScriptableRenderer.h"

Camera::Camera()
	: m_pos(glm::vec3()), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_fov(3.1415926535f / 2.0f), m_aspect(1.0f), m_nearPlane(0.1f), m_farPlane(1000.0f)
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& front, const float& fov, const float& aspect, const float& nearPlane, const float& farPlane)
	: m_pos(position), m_front(front), m_fov(fov), m_aspect(aspect), m_nearPlane(nearPlane), m_farPlane(farPlane)
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::Move(direction dir, float dis)
{
	switch (dir)
	{
		case direction::FORWARD:
		{
			m_pos += dis * glm::vec3(0.0f, 0.0f, -1.0f);
			break;
		}
		case direction::BACKWARD:
		{
			m_pos += dis * glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		}
		case direction::LEFT:
		{
			m_pos += dis * glm::vec3(-1.0f, 0.0f, 0.0f);
			break;
		}
		case direction::RIGHT:
		{
			m_pos += dis * glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		}
		case direction::UP:
		{
			m_pos += dis * glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}
		case direction::DOWN:
		{
			m_pos += dis * glm::vec3(0.0f, -1.0f, 0.0f);
		}
	}
	UpdateViewMatrix();
}

glm::mat4x4 Camera::GetViewMatrix()
{
	UpdateViewMatrix();
	return m_viewMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix()
{
	UpdateProjectionMatrix();
	return m_projectionMatrix;
}

void Camera::SetRenderer(ScriptableRenderer* renderer)
{
	m_renderer = renderer;
}

ScriptableRenderer* Camera::GetRenderer()
{
	return m_renderer;
}

CameraData* Camera::GetData()
{
	CameraData *data = (CameraData*)WEngine::Allocator::Get()->Allocate(sizeof(CameraData));
	::new (data) CameraData();

	data->camera = this;
	data->MatrixV = this->GetViewMatrix();
	data->MatrixP = this->GetProjectionMatrix();
	data->MatrixVP = data->MatrixP * data->MatrixV;

	return data;
}

void Camera::UpdateViewMatrix()
{
	//m_viewMatrix = Matrix4x4f::GetIdentityMatrix();
	//LookAt(m_viewMatrix, m_pos, Vector3(), Vector3(0.0f, 1.0f, 0.0f));
	m_viewMatrix =  glm::lookAt(m_pos, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateProjectionMatrix()
{
	//m_projectionMatrix = Matrix4x4f::GetIdentityMatrix();
	//PerspectiveProjection(m_projectionMatrix, m_fov, m_aspect, m_nearPlane, m_farPlane);
	m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
}
