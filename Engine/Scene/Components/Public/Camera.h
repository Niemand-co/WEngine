#pragma once
#include "Scene/Components/Public/Component.h"

class GameObject;
class ScriptableRenderer;
class RHITextureView;
class RHITexture;
struct CameraData;

struct RenderTarget
{
	RHITextureView *pColorTexture;
	RHITextureView *pDepthTexture;
	bool isChanged;
	unsigned int leftCount = 0;
	void SetupInformer(unsigned int count) { leftCount += count; }
	bool Informing() { if(leftCount == 0) return false; else { --leftCount; return true; } }
};

class Camera : public Component
{
	ENABLE_REFLECTION

public:

	typedef Camera type;

	Camera(GameObject* pGameObject, const float& fov = 3.1415926535f / 2.0f, const float& aspect = 1.0f, const float& nearPlane = 0.1f, const float& farPlane = 1000.0f);

	~Camera() = default;

	glm::mat4x4 GetViewMatrix();

	glm::mat4x4 GetProjectionMatrix();

	void SetRenderer(ScriptableRenderer *renderer);

	inline RenderTarget& GetRenderTarget(unsigned int index) { return m_rendertargets[index]; }

	ScriptableRenderer* GetRenderer();

	CameraData* GetData();

	void Move(Direction dir, float dis);

	void Rotate(RotateDirection dir, float dis);

	void RecreateRenderTarget(unsigned int width, unsigned int height);

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

	std::vector<RenderTarget> m_rendertargets;

	std::vector<RHITexture*> m_textureResources;

};

struct CameraData
{
	Camera *camera;
	glm::vec3 Position;
	glm::mat4x4 MatrixV;
	glm::mat4x4 MatrixP;
	glm::mat4x4 MatrixVP;
	RenderTarget *pRenderTarget;
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
				Field{"m_isOrthographic", &Camera::m_isOrthographic, AttrList{}},
				Field{"m_nearPlane", &Camera::m_nearPlane, AttrList{}},
				Field{"m_farPlane", &Camera::m_farPlane, AttrList{}},
				Field{"m_fov", &Camera::m_fov, AttrList{}},
				Field{"m_aspect", &Camera::m_aspect, AttrList{}},
				Field{"m_viewMatrix", &Camera::m_viewMatrix, AttrList{}},
				Field{"m_projectionMatrix", &Camera::m_projectionMatrix, AttrList{}},
				Field{"m_renderer", &Camera::m_renderer, AttrList{}},
				Field{"GetViewMatrix", &Camera::GetViewMatrix, AttrList{}},
				Field{"GetProjectionMatrix", &Camera::GetProjectionMatrix, AttrList{}},
				Field{"SetRenderer", &Camera::SetRenderer, AttrList{}},
				Field{"GetRenderer", &Camera::GetRenderer, AttrList{}},
				Field{"GetData", &Camera::GetData, AttrList{}},
				Field{"Move", &Camera::Move, AttrList{}},
				Field{"Rotate", &Camera::Rotate, AttrList{}},
			};
		};
	}
}