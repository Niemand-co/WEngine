#pragma once
#include "Scene/Components/Public/Component.h"

class TransformComponent : public Component
{

	friend class GameObject;

public:

	typedef TransformComponent type;

	TransformComponent(GameObject* pGameObject);

	~TransformComponent() = default;

	virtual void ShowInInspector() override;

	void SetScale(glm::vec3 scale);

	void SetRotate(glm::vec3 rotate);

	void SetPosition(glm::vec3 position);

	glm::vec3 GetPosition();

	glm::mat4 GetTranslateMatrix();

	glm::mat4 GetRotateMatrix();

	glm::mat4 GetLocalToWorldMatrix();

	glm::mat4 GetWorldToLocalMatrix();

	glm::vec3 GetForward();

	void Move(Direction dir, float dis);

	void Move(glm::vec3 displacement);

	void Rotate(RotateDirection dir, float dis);

private:

	glm::vec3 m_forward;

	glm::vec3 m_scale;

	glm::vec3 m_rotate;

	glm::vec3 m_position;

	glm::mat4 m_localToWorldMatrix;
};