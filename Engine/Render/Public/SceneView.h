#pragma once
#include "Scene/Components/Public/CameraComponent.h"

struct WViewMatrices
{
	WViewMatrices(CameraInfo* Camera)
	{
		ViewMatrix = Camera->MatrixV;
		ProjectionMatrix = Camera->MatrixP;
		ViewProjectionMatrix = Camera->MatrixVP;
		InvViewProjectionMatrix = glm::inverse(ViewProjectionMatrix);
	}

	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 InvViewProjectionMatrix = glm::mat4(1.0f);
	glm::vec2 Rect = glm::vec2();
};

struct WSceneViewInfo
{
	WViewMatrices ViewMatrices;

	glm::vec3 ViewLocation;

	glm::vec3 ViewDirection;

	class WSceneViewFamily* Family = nullptr;

	WSceneViewInfo(CameraInfo *Camera);

	void ComputeFrustum(WEngine::WArray<glm::vec3>& OutFrustum);

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}
};

class WSceneViewFamily
{
public:

	WSceneViewFamily(class WViewport *InViewport, class RScene *InScene);

private:

	RScene *Scene;

	class WRenderTarget *RenderTarget;

	WEngine::WArray<WSceneViewInfo> Views;

	friend class SceneRenderer;
	friend class DeferredRenderer;

};