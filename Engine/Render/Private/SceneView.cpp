#include "pch.h"
#include "Render/Public/SceneView.h"
#include "Render/Public/Viewport.h"
#include "Render/Public/Scene.h"

WSceneViewInfo::WSceneViewInfo(CameraInfo* Camera)
	: ViewMatrices(Camera),
	  ViewLocation(Camera->Position),
	  Resolution(Camera->Resolution)
{
}

void WSceneViewInfo::ComputeFrustum(WEngine::WArray<glm::vec3>& OutFrustum)
{
	OutFrustum.Resize(8);

	WEngine::WArray<glm::vec3> NDC = 
	{
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },

		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
	};

	for (uint32 Index = 0; Index < 8; ++Index)
	{
		glm::vec4 Pos = ViewMatrices.InvViewProjectionMatrix * glm::vec4(NDC[Index].x, NDC[Index].y, NDC[Index].z, 1.0f);
		OutFrustum[Index] = glm::vec3(Pos) / Pos.w;
	}
}

WSceneViewFamily::WSceneViewFamily(WViewport* InViewport, RScene* InScene)
	: Scene(InScene),
	  RenderTarget(InViewport)
{
	const WEngine::WArray<CameraInfo*>& Cameras = Scene->GetCameras();
	Views.Reserve(Cameras.Size());
	for (uint32 CameraIndex = 0; CameraIndex < Cameras.Size(); ++CameraIndex)
	{
		Views.Push(new WSceneViewInfo(Cameras[CameraIndex]));
	}
}
