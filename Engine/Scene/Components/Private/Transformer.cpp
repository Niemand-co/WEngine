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
	return glm::inverse(GetWorldToLocalMatrix());
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

