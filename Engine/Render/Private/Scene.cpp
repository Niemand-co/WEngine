#include "pch.h"
#include "Render/Public/Scene.h"
#include "Scene/Components/Public/InstancedStaticMeshComponent.h"
#include "Scene/Components/Public/LightComponent.h"
#include "Scene/Components/Public/CameraComponent.h"

RScene* RScene::g_activeScene = nullptr;

RScene::RScene()
{
}

RScene::~RScene()
{
}

void RScene::AddPrimitive(PrimitiveComponent* primitive)
{
	m_addedPrimitives.Add(primitive->GetPrimitiveInfo());
}

void RScene::RemovePrimitive(PrimitiveComponent* primitive)
{
	m_removedPrimitives.Remove(primitive->GetPrimitiveInfo());
}

void RScene::UpdatePrimitiveInfosForScene()
{
	WEngine::WArray<PrimitiveInfo*> AddedPrimitives = m_addedPrimitives.Array();
	m_addedPrimitives.Clear();

	WEngine::WArray<PrimitiveInfo*> RemovedPrimitives = m_removedPrimitives.Array();
	m_removedPrimitives.Clear();

	for (PrimitiveInfo* removedInfo : RemovedPrimitives)
	{
		uint32 size = m_lights.Size();
		uint32 index = 0;
		for (; index < size && m_primitives[index] == removedInfo; ++index);
		if (index >= size)continue;
		while (index < size)
		{
			uint32 removedIndex = index;
			for (; index < size && m_primitives[index]->type == removedInfo->type; ++index);
			WEngine::Swap(&m_primitives[index - 1], &m_primitives[removedIndex]);
		}
		m_primitives.Resize(size - 1);
	}

	for (PrimitiveInfo* addedInfo : AddedPrimitives)
	{
		m_primitives.Push(addedInfo);
		uint32 size = m_primitives.Size();
		if (size == 1)continue;
		while (m_primitives[size - 1]->type != m_primitives[size - 2]->type)
		{
			uint32 index = size - 2;
			for (; index >= 0 && m_primitives[index]->type == m_primitives[size - 1]->type; --index);
			if (index == 0 && m_primitives[index]->type != m_primitives[size - 1]->type)break;
			WEngine::Swap(&m_primitives[index + 1], &m_primitives[size - 1]);
		}
	}

	WEngine::WArray<PrimitiveInfo*>& primitives = m_primitives;

	PrimitiveInfo::AddToScene(GetRenderCommandList(), this, m_primitives);
}

void RScene::AddLight(LightComponent* light)
{
	m_addedLights.Add(light->GetLightInfo());
}

void RScene::RemoveLight(LightComponent* light)
{
	m_removedLights.Add(light->GetLightInfo());
}

void RScene::UpdateLightInfosForScene()
{
	WEngine::WArray<LightInfo*> AddedLights = m_addedLights.Array();
	m_addedLights.Clear();

	WEngine::WArray<LightInfo*> RemovedLights = m_removedLights.Array();
	m_removedLights.Clear();

	for (LightInfo* removedInfo : RemovedLights)
	{
		uint32 size = m_lights.Size();
		uint32 index = 0;
		for (; index < size && m_lights[index] == removedInfo; ++index);
		if(index >= size)continue;
		while (index < size)
		{
			uint32 removedIndex = index;
			for (; index < size && m_lights[index]->type == removedInfo->type; ++index);
			WEngine::Swap(&m_lights[index - 1], &m_lights[removedIndex]);
		}
		m_lights.Resize(size - 1);
	}

	for (LightInfo* addedInfo : AddedLights)
	{
		m_lights.Push(addedInfo);
		uint32 size = m_lights.Size();
		if(size == 1)continue;
		while (m_lights[size - 1]->type != m_lights[size - 2]->type)
		{
			uint32 index = size - 2;
			for (; index >= 0 && m_lights[index]->type == m_lights[size - 1]->type; --index);
			if(index == 0 && m_lights[index]->type != m_lights[size - 1]->type)break;
			WEngine::Swap(&m_lights[index + 1], &m_lights[size - 1]);
		}
	}
}

void RScene::AddCamera(CameraComponent* camera)
{
	m_addedCameras.Add(camera->GetCameraInfo());
}

void RScene::RemoveCamera(CameraComponent* camera)
{
	m_removedCameras.Add(camera->GetCameraInfo());
}

void RScene::UpdateCameraInfosForScene()
{
	WEngine::WArray<CameraInfo*> AddedCameras = m_addedCameras.Array();
	m_addedCameras.Clear();

	WEngine::WArray<CameraInfo*> RemovedCameras = m_removedCameras.Array();
	m_removedCameras.Clear();

	for (CameraInfo* removedInfo : RemovedCameras)
	{
		uint32 size = m_cameras.Size();
		uint32 index = 0;
		for (; index < size && m_cameras[index] == removedInfo; ++index);
		if (index >= size)continue;
		while (index < size)
		{
			uint32 removedIndex = index;
			for (; index < size && m_cameras[index]->type == removedInfo->type; ++index);
			WEngine::Swap(&m_cameras[index - 1], &m_cameras[removedIndex]);
		}
		m_cameras.Resize(size - 1);
	}

	for (CameraInfo* addedInfo : AddedCameras)
	{
		m_cameras.Push(addedInfo);
		uint32 size = m_cameras.Size();
		if (size == 1)continue;
		while (m_cameras[size - 1]->type != m_cameras[size - 2]->type)
		{
			uint32 index = size - 2;
			for (; index >= 0 && m_cameras[index]->type == m_cameras[size - 1]->type; --index);
			if (index == 0 && m_cameras[index]->type != m_cameras[size - 1]->type)break;
			WEngine::Swap(&m_cameras[index + 1], &m_cameras[size - 1]);
		}
	}
}

void RScene::StartFrame()
{
}
