#include "pch.h"
#include "Scene/Components/Public/Camera.h"
#include "Scene/Public/GameObject.h"
#include "Render/Public/ScriptableRenderer.h"

Camera::Camera(GameObject* pGameObject, const float& fov, const float& aspect, const float& nearPlane, const float& farPlane)
	: m_fov(fov), m_aspect(aspect), m_nearPlane(nearPlane), m_farPlane(farPlane), m_pGameObject(pGameObject)
{
	m_type = Component::ComponentType::Camera; 
	UpdateProjectionMatrix();
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
	data->Position = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>())->GetPosition();
	data->MatrixV = this->GetViewMatrix();
	data->MatrixP = this->GetProjectionMatrix();
	data->MatrixVP = data->MatrixP * data->MatrixV;

	return data;
}

void Camera::Move(direction dir, float dis)
{
	glm::vec3 displacement = glm::vec3(0.0f);
	switch (dir)
	{
	case Camera::direction::FORWARD:
		displacement = dis * glm::vec3(0.0f, 0.0f, -1.0f);
		break;
	case Camera::direction::BACKWARD:
		displacement = dis * glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	case Camera::direction::LEFT:
		displacement = dis * glm::vec3(-1.0f, 0.0f, 0.0f);
		break;
	case Camera::direction::RIGHT:
		displacement = dis * glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case Camera::direction::UP:
		displacement = dis * glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case Camera::direction::DOWN:
		displacement = dis * glm::vec3(0.0f, -1.0f, 0.0f);
		break;
	default:
		break;
	}
	Transformer *transformer = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>());
	transformer->SetPosition(transformer->GetPosition() + displacement);
}

void Camera::UpdateViewMatrix()
{
	glm::vec3 position = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>())->GetPosition();
	glm::vec3 forward = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>())->GetForward();
	m_viewMatrix = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f) /*position  + forward */, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateProjectionMatrix()
{
	//m_projectionMatrix = Matrix4x4f::GetIdentityMatrix();
	//PerspectiveProjection(m_projectionMatrix, m_fov, m_aspect, m_nearPlane, m_farPlane);
	m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
}
