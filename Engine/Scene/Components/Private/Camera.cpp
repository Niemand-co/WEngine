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

void Camera::Move(Direction dir, float dis)
{
	Transformer *transformer = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>());
	transformer->Move(dir, dis);
}

void Camera::Rotate(RotateDirection dir, float dis)
{
	Transformer* transformer = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>());
	transformer->Rotate(dir, dis);
}

void Camera::UpdateViewMatrix()
{
	glm::vec3 position = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>())->GetPosition();
	glm::vec3 forward = static_cast<Transformer*>(m_pGameObject->GetComponent<Component::ComponentType::Transformer>())->GetForward();
	m_viewMatrix = glm::lookAt(position, position  + forward, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateProjectionMatrix()
{
	//m_projectionMatrix = Matrix4x4f::GetIdentityMatrix();
	//PerspectiveProjection(m_projectionMatrix, m_fov, m_aspect, m_nearPlane, m_farPlane);
	m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
}
