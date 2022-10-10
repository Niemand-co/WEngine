#include "pch.h"
#include "Scene/Components/Public/Transformer.h"

Transformer::Transformer()
{
	m_type = Component::ComponentType::Transformer;
	m_localToWorldMatrix = glm::mat4(1.0f);
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
	glm::scale(m_localToWorldMatrix, m_scale);
	glm::rotate(m_localToWorldMatrix, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::rotate(m_localToWorldMatrix, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::rotate(m_localToWorldMatrix, m_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::translate(m_localToWorldMatrix, m_position);
	return m_localToWorldMatrix;
}

glm::mat4 Transformer::GetWorldToLocalMatrix()
{
	return glm::inverse(GetWorldToLocalMatrix());
}

