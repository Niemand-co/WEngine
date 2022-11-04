#pragma once
#include "Scene/Components/Public/Component.h"

class GameObject;
class ScriptableRenderer;
struct CameraData;

class Camera : public Component
{
	ENABLE_REFLECTION

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

namespace WEngine
{
	namespace SRefl
	{
		template<>
		struct TypeInfo<Camera> : public TypeInfoBase<Camera, Component>
		{
			static constexpr std::string_view name = "Camera";

			static constexpr FieldList list = 
			{
				Field{"m_isOrthographic", &Camera::m_isOrthographic},
				Field{"m_nearPlane", &Camera::m_nearPlane},
				Field{"m_farPlane", &Camera::m_farPlane},
				Field{"m_fov", &Camera::m_fov},
				Field{"m_aspect", &Camera::m_aspect},
				Field{"m_viewMatrix", &Camera::m_viewMatrix},
				Field{"m_projectionMatrix", &Camera::m_projectionMatrix},
				Field{"m_renderer", &Camera::m_renderer},
				Field{"GetViewMatrix", &Camera::GetViewMatrix},
				Field{"GetProjectionMatrix", &Camera::GetProjectionMatrix},
				Field{"SetRenderer", &Camera::SetRenderer},
				Field{"GetRenderer", &Camera::GetRenderer},
				Field{"GetData", &Camera::GetData},
				Field{"Move", &Camera::Move},
				Field{"Rotate", &Camera::Rotate},
			};
		};
	}
}