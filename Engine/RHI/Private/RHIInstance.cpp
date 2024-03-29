#include "pch.h"
#include "RHI/Public/RHIInstance.h"
#include "RHI/Public/RHIGPU.h"
#include "Render/Descriptor/Public/RHIInstanceDescriptor.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"

RHIInstance* RHIInstance::g_instance = nullptr;

RHIInstance* RHIInstance::CreateInstance(RHIInstanceDescriptor *descriptor)
{
	if(g_instance != nullptr)
		return g_instance;

	switch (descriptor->backend)
	{
		case RHIBackend::Vulkan:
		{
			g_instance = (RHIInstance*)WEngine::Allocator::Get()->Allocate(sizeof(Vulkan::VulkanInstance));
			::new (g_instance) Vulkan::VulkanInstance(descriptor->enableDebugLayer, descriptor->enableGPUValidator);
			g_instance->InitializeGPU();
			return g_instance;
		}
	}

	return nullptr;
}

RHIGPU* RHIInstance::GetGPU(unsigned int gpuID)
{
	return m_gpus[gpuID];
}

RHISurface* RHIInstance::GetSurface()
{
	return m_surface;
}
