#pragma once
#include "Scene/Components/Public/Component.h"

class ScriptableRenderer;
struct CameraData;

class Camera : public Component
{
public:
	enum class direction
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	Camera();

	Camera(const glm::vec3& position, const glm::vec3& front, const float& fov = 90.f, const float& aspect = 1.0f, const float& nearPlane = 0.1f, const float& farPlane = 1000.0f);

	~Camera() = default;

	void Move(direction dir, float dis);

	glm::vec3 GetPos(){return m_pos;}

	glm::mat4x4 GetViewMatrix();

	glm::mat4x4 GetProjectionMatrix();

	void SetRenderer(ScriptableRenderer *renderer);

	ScriptableRenderer* GetRenderer();

	CameraData* GetData();

private:

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

public:

	bool m_isOrthographic;

	float m_nearPlane;

	float m_farPlane;

	float m_fov;

	float m_aspect;

	glm::vec3 m_pos;

	glm::vec3 m_front;

private:

	glm::mat4x4 m_viewMatrix;

	glm::mat4x4 m_projectionMatrix;

	ScriptableRenderer *m_renderer;

};

struct CameraData
{
	Camera *camera;
	glm::mat4x4 MatrixV;
	glm::mat4x4 MatrixP;
	glm::mat4x4 MatrixVP;
};