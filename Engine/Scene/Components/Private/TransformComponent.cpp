#include "pch.h"
#include "Scene/Components/Public/TransformComponent.h"

TransformComponent::TransformComponent(GameObject* pGameObject)
	: Component(pGameObject)
{
	m_type = Component::ComponentType::Transformer;
	m_localToWorldMatrix = glm::mat4(1.0f);
	m_rotate = glm::vec3(0.0f);
	m_position = glm::vec3(0.0f);
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_scale = glm::vec3(1.f, 1.f, 1.f);
}

void TransformComponent::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Transformer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float lineHeight = ImGui::GetFontSize() * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::Columns(2); ImGui::SetColumnWidth(0, 80.0f); 
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
		
		ImGui::Text("Position"); ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if(ImGui::Button("X:", buttonSize)) { m_position.x = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##PositionX", &m_position.x, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth(); ImGui::SameLine(); 
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		if(ImGui::Button("Y:", buttonSize)) { m_position.y = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##PositionY", &m_position.y, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth(); ImGui::SameLine();
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if(ImGui::Button("Z:", buttonSize)) { m_position.z = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##PositionZ", &m_position.z, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		ImGui::NextColumn();

		ImGui::Text("Rotation"); ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if(ImGui::Button("X:", buttonSize)) { m_rotate.x = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##RotateX", &m_rotate.x, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth(); ImGui::SameLine();
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		if(ImGui::Button("Y:", buttonSize)) { m_rotate.y = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##RotateY", &m_rotate.y, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth(); ImGui::SameLine();
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if(ImGui::Button("Z:", buttonSize)) { m_rotate.z = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##RotateZ", &m_rotate.z, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		ImGui::NextColumn();

		ImGui::Text("Scale"); ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if(ImGui::Button("X:", buttonSize)) { m_scale.x = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##ScaleX", &m_scale.x, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth(); ImGui::SameLine();
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		if(ImGui::Button("Y:", buttonSize)) { m_scale.y = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##ScaleY", &m_scale.y, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth(); ImGui::SameLine();
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if(ImGui::Button("Z:", buttonSize)) { m_scale.z = 0.0f; } ImGui::SameLine(); ImGui::DragFloat("##ScaleZ", &m_scale.z, 0.1f, 0.0f, 0.0f, "%.2f"); ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		
	}
}

void TransformComponent::SetScale(glm::vec3 scale)
{
	m_scale = scale;
}

void TransformComponent::SetRotate(glm::vec3 rotate)
{
	m_rotate = rotate;
}

void TransformComponent::SetPosition(glm::vec3 translate)
{
	m_position = translate;
}

glm::vec3 TransformComponent::GetPosition()
{
	return m_position;
}

glm::mat4 TransformComponent::GetTranslateMatrix()
{
	return glm::translate(glm::mat4(1.0f), m_position);
}

glm::mat4 TransformComponent::GetRotateMatrix()
{
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.x), glm::vec3(1.0f, 0.0f, 0.0f))
						   * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.y), glm::vec3(0.0f, 1.0f, 0.0f))
						   * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return rotateMatrix;
}

glm::mat4 TransformComponent::GetLocalToWorldMatrix()
{
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.x), glm::vec3(1.0f, 0.0f, 0.0f))
						   * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.y), glm::vec3(0.0f, 1.0f, 0.0f))
						   * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), m_position);
	
	return translateMatrix * rotateMatrix * scaleMatrix;
}

glm::mat4 TransformComponent::GetWorldToLocalMatrix()
{
	return glm::inverse(GetLocalToWorldMatrix());
}

glm::vec3 TransformComponent::GetForward()
{
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.x), glm::vec3(1.0f, 0.0f, 0.0f))
		   				   * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.y), glm::vec3(0.0f, 1.0f, 0.0f))
						   * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_forward = glm::normalize(glm::vec3(rotateMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
	return m_forward;
}

void TransformComponent::Move(Direction dir, float dis)
{
	glm::vec3 worldUP = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(m_forward, worldUP));

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

void TransformComponent::Move(glm::vec3 displacement)
{
	m_position += displacement;
}

void TransformComponent::Rotate(RotateDirection dir, float dis)
{
	switch (dir)
	{
	case RotateDirection::Pitch:
		m_rotate.x += dis;
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
}

