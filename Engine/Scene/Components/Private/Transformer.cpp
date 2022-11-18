#include "pch.h"
#include "Scene/Components/Public/Transformer.h"

Transformer::Transformer(GameObject* pGameObject)
	: Component(pGameObject)
{
	m_type = Component::ComponentType::Transformer;
	m_localToWorldMatrix = glm::mat4(1.0f);
	m_rotate = glm::vec3(0.0f);
	m_position = glm::vec3(0.0f);
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_scale = glm::vec3(1.f, 1.f, 1.f);
}

void Transformer::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Transformer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(80.0f);
		ImGui::Text("Position ");
		ImGui::PushItemWidth(20.0f);
		ImGui::SameLine();ImGui::Text("X:"); ImGui::SameLine(); ImGui::DragFloat("##PositionX", &m_position.x, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine(); ImGui::Text("Y:"); ImGui::SameLine(); ImGui::DragFloat("##PositionY", &m_position.y, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine(); ImGui::Text("Z:"); ImGui::SameLine(); ImGui::DragFloat("##PositionZ", &m_position.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::Text("Rotation ");
		ImGui::PushItemWidth(20.0f);
		ImGui::SameLine(); ImGui::Text("X:"); ImGui::SameLine(); ImGui::DragFloat("##RotateX", &m_rotate.x, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine(); ImGui::Text("Y:"); ImGui::SameLine(); ImGui::DragFloat("##RotateY", &m_rotate.y, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine(); ImGui::Text("Z:"); ImGui::SameLine(); ImGui::DragFloat("##RotateZ", &m_rotate.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::Text("Scale    ");
		ImGui::PushItemWidth(20.0f);
		ImGui::SameLine(); ImGui::Text("X:"); ImGui::SameLine(); ImGui::DragFloat("##ScaleX", &m_scale.x, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine(); ImGui::Text("Y:"); ImGui::SameLine(); ImGui::DragFloat("##ScaleY", &m_scale.y, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine(); ImGui::Text("Z:"); ImGui::SameLine(); ImGui::DragFloat("##ScaleZ", &m_scale.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();
	}
}

void Transformer::SetScale(glm::vec3 scale)
{
	m_scale = scale;
}

void Transformer::SetRotate(glm::vec3 rotate)
{
	m_rotate = rotate;
}

void Transformer::SetPosition(glm::vec3 translate)
{
	m_position = translate;
}

glm::vec3 Transformer::GetPosition()
{
	return m_position;
}

glm::mat4 Transformer::GetRotateMatrix()
{
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
	rotateMatrix = glm::rotate(rotateMatrix, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, m_rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, m_rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
	return rotateMatrix;
}

glm::mat4 Transformer::GetLocalToWorldMatrix()
{
	m_localToWorldMatrix = glm::mat4(1.0f);
	m_localToWorldMatrix = glm::scale(m_localToWorldMatrix, m_scale);
	m_localToWorldMatrix = glm::rotate(m_localToWorldMatrix, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_localToWorldMatrix = glm::rotate(m_localToWorldMatrix, m_rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_localToWorldMatrix = glm::rotate(m_localToWorldMatrix, m_rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
	m_localToWorldMatrix = glm::translate(m_localToWorldMatrix, m_position);
	return m_localToWorldMatrix;
}

glm::mat4 Transformer::GetWorldToLocalMatrix()
{
	return glm::inverse(GetLocalToWorldMatrix());
}

glm::vec3 Transformer::GetForward()
{
	return m_forward;
}

void Transformer::Move(Direction dir, float dis)
{
	glm::vec3 worldUP = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::cross(m_forward, worldUP);

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
	m_position += displacement;
}

void Transformer::Rotate(RotateDirection dir, float dis)
{
	switch (dir)
	{
	case RotateDirection::Pitch:
		m_rotate.x += dis;
		m_rotate.x = m_rotate.x > 89.0f ? 89.0f : (m_rotate.x < -89.0f ? -89.0f : m_rotate.x);
		break;
	case RotateDirection::Yaw:
		m_rotate.y += dis;
		break;
	case RotateDirection::Roll:
		m_rotate.z += dis;
		break;
	default:
		break;
	}
	
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(m_rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(m_rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(m_rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_forward = glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) * rotateMatrix);
}

