#pragma once
#include "Scene/Components/Public/Component.h"

class GameObject;
class ScriptableRenderer;
struct CameraData;

class Camera : public Component
{
public:

	Camera(GameObject* pGameObject, const float& fov = 3.1415926535f / 2.0f, const float& aspect = 1.0f, const float& nearPlane = 0.1f, const float& farPlane = 1000.0f);

	~Camera() = default;

	glm::mat4x4 GetViewMatrix();

	glm::mat4x4 GetProjectionMatrix();

	void SetRenderer(ScriptableRenderer *renderer);

	ScriptableRenderer* GetRenderer();

	CameraData* GetData();

	void Move(Direction dir, float dis);

	void Rotate(RotateDirection dir, float dis);

private:

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

public:

	bool m_isOrthographic;

	float m_nearPlane;

	float m_farPlane;

	float m_fov;

	float m_aspect;

private:

	GameObject *m_pGameObject;

	glm::mat4x4 m_viewMatrix;

	glm::mat4x4 m_projectionMatrix;

	ScriptableRenderer *m_renderer;

};
struct CameraData
{
	Camera *camera;
	glm::vec3 Position;
	glm::mat4x4 MatrixV;
	glm::mat4x4 MatrixP;
	glm::mat4x4 MatrixVP;
};