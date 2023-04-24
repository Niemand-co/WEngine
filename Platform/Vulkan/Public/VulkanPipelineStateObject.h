#pragma once
#include "RHI/Public/RHIPipelineStateObject.h"

namespace Vulkan
{

	class VulkanGraphicsPipelineStateObject : public RHIPipelineStateObject
	{
	public:

		VulkanGraphicsPipelineStateObject(class VulkanDevice* pInDevice, VkGraphicsPipelineCreateInfo* pInfo);

		virtual ~VulkanGraphicsPipelineStateObject();

		virtual void Bind(class RHICommandBuffer *CmdBuffer) override;

		VkPipeline GetHandle() const { return Pipeline; }

	private:

		class VulkanDevice *pDevice;
		class VulkanRenderPass *RenderPass;
		class VulkanLayout *Layout;

		VkPipeline Pipeline;

	};

	class VulkanComputePipelineStateObject : public RHIPipelineStateObject
	{

	};

	class VulkanPipelineStateManager
	{
	public:

		static VulkanGraphicsPipelineStateObject* GetGraphicsPipelineState(uint32 ID)
		{
			if (GraphicsPipelines.Find(ID))
			{
				return GraphicsPipelines[ID];
			}
			return nullptr;
		}

		static VulkanComputePipelineStateObject* GetComputePipelineState(uint32 ID)
		{
			if (ComputePipelines.Find(ID))
			{
				return ComputePipelines[ID];
			}
			return nullptr;
		}

		static void AddGraphicsPipelineState(uint32 ID, VulkanGraphicsPipelineStateObject* Pipeline)
		{
			GraphicsPipelines.Insert(ID, Pipeline);
		}

		static void AddComputePipelineState(uint32 ID, VulkanComputePipelineStateObject* Pipeline)
		{
			ComputePipelines.Insert(ID, Pipeline);
		}

	private:

		static WEngine::WHashMap<uint32, VulkanGraphicsPipelineStateObject*> GraphicsPipelines;

		static WEngine::WHashMap<uint32, VulkanComputePipelineStateObject*> ComputePipelines;

	};

	class VulkanCommonPipelineDescriptorState : public VulkanResource
	{
	public:

		VulkanCommonPipelineDescriptorState();

		virtual ~VulkanCommonPipelineDescriptorState();



	};

	class VulkanGraphicsPipelineDescriptorState : public VulkanCommonPipelineDescriptorState
	{

	};

	class VulkanComputePipelineDescriptorState : public VulkanCommonPipelineDescriptorState
	{

	};

}