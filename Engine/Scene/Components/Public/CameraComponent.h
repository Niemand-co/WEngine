#pragma once
#include "Scene/Components/Public/Component.h"

class GameObject;
class ScriptableRenderer;
class RHITextureView;
class RHITexture;
struct CameraData;

struct CameraRenderTarget
{
	RHITextureView *pColorTexture;
	RHITextureView *pDepthTexture;
};

class CameraComponent : public Component
{
	ENABLE_REFLECTION

	friend class GameObject;

public:

	enum { type = 3 };

	CameraComponent(GameObject* pGameObject, const float& fov = 90.0f, const float& aspect = 1.0f, const float& nearPlane = 0.01f, const float& farPlane = 1000.0f);

	~CameraComponent() = default;

	virtual void ShowInInspector() override;

	glm::mat4x4 GetViewMatrix();

	glm::mat4x4 GetProjectionMatrix();

	glm::vec3 GetForward() const { return m_forward; }

	void SetRenderer(ScriptableRenderer *renderer);

	inline CameraRenderTarget& GetRenderTarget(unsigned int index) { return m_rendertargets[index]; }

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

	WEngine::WArray<CameraRenderTarget> m_rendertargets;

	WEngine::WArray<RHITexture*> m_textureResources;

	CameraData *m_pData;

	glm::vec3 m_forward;

	float phi;

	float theta;

};

struct CameraInfo
{
	CameraInfo(CameraComponent* camera)
		: Position(camera->GetOwner()->GetComponent<TransformComponent>()->GetPosition()),
		  MatrixV(camera->GetViewMatrix()),
		  MatrixP(camera->GetProjectionMatrix()),
		  MatrixVP(MatrixP * MatrixV),
		  Fov(camera->m_fov),
		  FarClip(camera->m_farPlane),
		  NearClip(camera->m_nearPlane),
		  Aspect(camera->m_aspect),
		  Owner(camera->GetOwner())
	{

	}

	glm::vec3 Position;
	
	glm::mat4x4 MatrixV;
	
	glm::mat4x4 MatrixP;
	
	glm::mat4x4 MatrixVP;
	
	CameraRenderTarget *pRenderTarget;
	
	float Fov;
	
	float FarClip;
	
	float NearClip;
	
	float Aspect;

	GameObject *Owner;
};

//namespace WEngine
//{
//	namespace SRefl
//	{
//		template<>
//		struct TypeInfo<Camera> : public TypeInfoBase<Camera, Component>
//		{
//			//static constexpr WString name = "Camera";
//
//			static constexpr FieldList list = 
//			{
//				Field{"m_isOrthographic", &Camera::m_isOrthographic, AttrList{}},
//				Field{"m_nearPlane", &Camera::m_nearPlane, AttrList{}},
//				Field{"m_farPlane", &Camera::m_farPlane, AttrList{}},
//				Field{"m_fov", &Camera::m_fov, AttrList{}},
//				Field{"m_aspect", &Camera::m_aspect, AttrList{}},
//				Field{"m_viewMatrix", &Camera::m_viewMatrix, AttrList{}},
//				Field{"m_projectionMatrix", &Camera::m_projectionMatrix, AttrList{}},
//				Field{"m_renderer", &Camera::m_renderer, AttrList{}},
//				Field{"GetViewMatrix", &Camera::GetViewMatrix, AttrList{}},
//				Field{"GetProjectionMatrix", &Camera::GetProjectionMatrix, AttrList{}},
//				Field{"SetRenderer", &Camera::SetRenderer, AttrList{}},
//				Field{"GetRenderer", &Camera::GetRenderer, AttrList{}},
//				Field{"GetData", &Camera::GetData, AttrList{}},
//				Field{"Move", &Camera::Move, AttrList{}},
//				Field{"Rotate", &Camera::Rotate, AttrList{}},
//			};
//		};
//	}
//}