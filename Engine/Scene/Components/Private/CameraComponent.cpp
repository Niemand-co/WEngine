#include "pch.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Render/Public/Scene.h"

CameraComponent::CameraComponent(GameObject* pGameObject, const float& fov, const float& aspect, const float& nearPlane, const float& farPlane)
	: m_fov(fov), m_aspect(aspect), m_nearPlane(nearPlane), m_farPlane(farPlane), m_renderer(nullptr), m_pInfo(nullptr), Component(pGameObject)
{
	phi = 0.0f;
	theta = 0.0f;
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

	RScene::GetActiveScene()->AddCamera(this);
}

void CameraComponent::ShowInInspector()
{
}

glm::mat4x4 CameraComponent::GetViewMatrix()
{
	UpdateViewMatrix();
	return m_viewMatrix;
}

glm::mat4x4 CameraComponent::GetProjectionMatrix()
{
	UpdateProjectionMatrix();
	return m_projectionMatrix;
}

ScriptableRenderer* CameraComponent::GetRenderer()
{
	return m_renderer;
}

CameraInfo* CameraComponent::GetCameraInfo()
{
	if (m_bMarkedDirty)
	{
		if (m_pInfo == nullptr)
		{
			m_pInfo = (CameraInfo*)NormalAllocator::Get()->Allocate(sizeof(CameraInfo));
		}
		::new (m_pInfo) CameraInfo(this);
		m_bMarkedDirty = false;
	}

	return m_pInfo;
}

void CameraComponent::Move(Direction dir, float dis)
{
	glm::vec3 right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	glm::vec3 displacement = glm::vec3(0.0f);
	switch (dir)
	{
	case Direction::FORWARD:
		displacement = dis * m_forward;
		break;
	case Direction::BACKWARD:
		displacement = dis * -m_forward;
		break;
	case Direction::LEFT:
		displacement = dis * -right;
		break;
	case Direction::RIGHT:
		displacement = dis * right;
		break;
	case Direction::UP:
		displacement = dis * glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case Direction::DOWN:
		displacement = dis * glm::vec3(0.0f, -1.0f, 0.0f);
		break;
	default:
		break;
	}

	m_pGameObject->GetComponent<TransformComponent>()->Move(displacement);
}

void CameraComponent::Rotate(RotateDirection dir, float dis)
{
	switch (dir)
	{
	case RotateDirection::Pitch:
		phi += dis;
		phi = phi > 89.0f ? 89.0f : (phi < -89.0f ? -89.0f : phi);
		break;
	case RotateDirection::Yaw:
		theta += dis;
		break;
	default:
		break;
	}
	m_forward.x = glm::cos(glm::radians(phi)) * glm::cos(glm::radians(theta));
	m_forward.y = glm::sin(glm::radians(phi));
	m_forward.z = -glm::cos(glm::radians(phi)) * glm::sin(glm::radians(theta));
}

void CameraComponent::UpdateViewMatrix()
{
	glm::vec3 position = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	m_viewMatrix = glm::lookAt(position, position  + m_forward, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CameraComponent::UpdateProjectionMatrix()
{
	//m_projectionMatrix = Matrix4x4f::GetIdentityMatrix();
	//PerspectiveProjection(m_projectionMatrix, m_fov, m_aspect, m_nearPlane, m_farPlane);
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_nearPlane, m_farPlane);
}
