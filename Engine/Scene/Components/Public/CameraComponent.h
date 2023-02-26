#pragma once
#include "Scene/Components/Public/Component.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Render/Renderer/Public/DeferredRenderer.h"

class GameObject;
class ScriptableRenderer;
class RHITextureView;
class RHITexture;
struct CameraInfo;

struct CameraRenderTarget
{
	RHITextureView *pColorTexture;
	RHITextureView *pDepthTexture;
};

class CameraComponent : public Component
{
	ENABLE_REFLECTION

	friend class GameObject;

	friend struct CameraInfo;

public:

	enum { type = 3 };

	CameraComponent(GameObject* pGameObject, const float& fov = 90.0f, const float& aspect = 1.0f, const float& nearPlane = 0.01f, const float& farPlane = 1000.0f);

	~CameraComponent() = default;

	virtual void ShowInInspector() override;

	glm::mat4x4 GetViewMatrix();

	glm::mat4x4 GetProjectionMatrix();

	glm::vec3 GetForward() const { return m_forward; }

	template<typename T>
	void SetRenderer(RScene* Scene);

	inline CameraRenderTarget& GetRenderTarget(unsigned int index) { return m_rendertargets[index]; }

	ScriptableRenderer* GetRenderer();

	CameraInfo* GetCameraInfo();

	glm::vec2& GetResolution() { return m_resolution; }

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

	WEngine::WArray<CameraRenderTarget> m_rendertargets;

	WEngine::WArray<RHITexture*> m_textureResources;

	CameraInfo *m_pInfo;

	glm::vec3 m_forward;

	float phi;

	float theta;

	glm::vec2 m_resolution;

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
		  Renderer(camera->m_renderer),
		  Owner(camera->GetOwner()),
		  bMarkedDirty(false)
	{

	}

	void MarkDirty() { bMarkedDirty = true; }

	void UpdateInfo()
	{
		if(!bMarkedDirty)
			return;

		::new (this) CameraInfo(Owner->GetComponent<CameraComponent>());
		bMarkedDirty = false;
	}

	enum { type = 0 };

	glm::vec3 Position;
	
	glm::mat4x4 MatrixV;
	
	glm::mat4x4 MatrixP;
	
	glm::mat4x4 MatrixVP;
	
	CameraRenderTarget *pRenderTarget;
	
	float Fov;
	
	float FarClip;
	
	float NearClip;
	
	float Aspect;

	ScriptableRenderer* Renderer;

	GameObject *Owner;

	uint8 bMarkedDirty : 1;
};

template<typename T>
void CameraComponent::SetRenderer(RScene* Scene)
{
	m_renderer = new T(this);
	SceneRenderer* renderer = dynamic_cast<SceneRenderer*>(m_renderer);
	if (renderer != nullptr)
		renderer->SetScene(Scene);
}

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