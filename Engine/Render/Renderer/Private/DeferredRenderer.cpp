#include "pch.h"
#include "Render/Renderer/Public/DeferredRenderer.h"
#include "Render/Public/Scene.h"
#include "Render/Public/Shader.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Scene/Components/Public/DirectionalLightComponent.h"
#include "Render/Public/SceneVisibility.h"
#include "Render/Passes/Public/DeferredBasePass.h"
#include "Render/Public/RenderDependencyGraph.h"
#include "Render/Public/RenderDependencyGraphResource.h"
#include "Render/Public/RenderDependencyGraphParameter.h"
#include "Render/Public/Viewport.h"
#include "RHI/Public/RHIViewport.h"

DeferredRenderer::DeferredRenderer(CameraComponent* pCamera, WViewport *Viewport)
	: SceneRenderer(pCamera, Viewport)
{
	GraphBuilder = new WRDGBuilder();
	SetupBasePass(Viewport);
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::Render()
{
	Scene->UpdateLightInfosForScene();

	InitView();

	RenderPrePass();

	RenderBasePass();

	RenderShadowPass();

	RenderLightPass();

	RenderSkybox();

	RenderTranslucent();

	RenderPostEffect();

	GraphBuilder->Execute();
}

void DeferredRenderer::InitView()
{
	ComputeVisibility();

	LightInfo *MainLight = Scene->GetMainLightInfo();
	if (MainLight)
	{
		WEngine::CascadedShadowMap::UpdateSplices(CSMMaps, m_pCamera->m_nearPlane, m_pCamera->m_farPlane);
		WEngine::CascadedShadowMap::UpdatePSSMMatrices(CSMMaps, glm::inverse(m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix()), static_cast<DirectionalLightInfo*>(MainLight)->LightDirection);
	}
}

void DeferredRenderer::RenderPrePass()
{
	
}

void DeferredRenderer::RenderBasePass()
{

}

void DeferredRenderer::RenderShadowPass()
{
}

void DeferredRenderer::RenderLightPass()
{
	RE_LOG("Render Light Pass");
}

void DeferredRenderer::RenderSkybox()
{
}

void DeferredRenderer::RenderTranslucent()
{
}

void DeferredRenderer::RenderPostEffect()
{
}

void DeferredRenderer::ComputeVisibility()
{
	const WEngine::WArray<PrimitiveInfo*>& OpaqueAndMaskPrimitives = Scene->GetOpaqueAndMaskPrimitives();
	const WEngine::WArray<PrimitiveInfo*>& TranslucentPrimitives = Scene->GetTranslucentPrimitives();

	WEngine::WArray<glm::vec3> Frustum(8);
	{
		TransformComponent *Transformer = m_pCamera->GetOwner()->GetComponent<TransformComponent>();
		glm::vec3 Forward = m_pCamera->GetForward();
		glm::vec3 NearCenter = Transformer->GetPosition() + Forward * m_pCamera->m_nearPlane;
		glm::vec3 FarCenter = Transformer->GetPosition() + Forward * m_pCamera->m_farPlane;
		glm::vec3 Up = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), Forward);
		glm::vec3 Right = glm::cross(Forward, Up);
		float arctanHalfFov = glm::tan(m_pCamera->m_fov * 0.5f);
		float NearHeight = arctanHalfFov * m_pCamera->m_nearPlane * 2.0f;
		float NearWidth = NearHeight * m_pCamera->m_aspect;
		float FarHeight = arctanHalfFov * m_pCamera->m_farPlane * 2.0f;
		float FarWidth = FarHeight * m_pCamera->m_aspect;
		glm::vec3 NearUp = Up * NearHeight;
		glm::vec3 NearRight = Right * NearWidth;
		glm::vec3 FarUp = Up * FarHeight;
		glm::vec3 FarRight = Right * FarWidth;

		Frustum[0] = NearCenter - NearUp - NearRight;
		Frustum[1] = NearCenter - NearUp + NearRight;
		Frustum[2] = NearCenter + NearUp + NearRight;
		Frustum[3] = NearCenter + NearUp - NearRight;

		Frustum[4] = FarCenter - FarUp - FarRight;
		Frustum[5] = FarCenter - FarUp + FarRight;
		Frustum[6] = FarCenter + FarUp + FarRight;
		Frustum[7] = FarCenter + FarUp - FarRight;
	}
	
	WEngine::WTaskGraph::Get()->ParallelFor(OpaqueAndMaskPrimitives.Size(), [this, &OpaqueAndMaskPrimitives, &Frustum](uint32 index)
	{
		PrimitiveInfo *info = OpaqueAndMaskPrimitives[index];
		const BoundingBox& box = info->Proxy->GetBoundingBox();

		if ((bUseBoxTest ? IsBoxInFrustum(Frustum, box.BoxMin, box.BoxMax) : true) && (bUseSphereTest ? IsSphereInFrustum(Frustum, glm::vec3(), 0.0f) : true))
		{
			Views[index].Visibility = true;
		}
		else
		{
			Views[index].Visibility = false;
		}
	});

}

void DeferredRenderer::SetupBasePass(WViewport* Viewport)
{
	glm::vec2 Resolution = m_pCamera->GetResolution();
	WRDGTexture* GBuffer0 = GraphBuilder->RegisterExternalTexture(Viewport->GetRHI()->GetRenderTarget());

	const WRDGTextureDesc DepthDesc = WRDGTextureDesc::GetTexture2DDesc(Format::D16_Unorm, { (uint32)Resolution.x, (uint32)Resolution.y, 0u }, { 0.0f, 0.0f, 0.0f, 0.0f });
	WRDGTexture* DepthBuffer = GraphBuilder->CreateTexture(DepthDesc, "Depth");

	DeferredBasePassParameters* Parameters = GraphBuilder->AllocateParameterStruct<DeferredBasePassParameters>();
	Parameters->RenderTarget.ColorTextures[0].Texture = GBuffer0;
	Parameters->RenderTarget.DepthStencilTexture.Texture = DepthBuffer;

	GraphBuilder->AddPass("BasePass", Parameters, [](RHIRenderCommandList& CmdList)
	{
		CmdList.DrawIndexedPrimitive(3, 0, 1);
	});
}
