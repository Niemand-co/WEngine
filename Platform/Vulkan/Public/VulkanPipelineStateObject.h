#pragma once
#include "RHI/Public/RHIPipelineStateObject.h"

namespace Vulkan
{

	struct GfxPipelineDesc
	{
		uint32 VertexInputKey;
		uint16 RasterizationSamples;
		uint32 Topology;
		uint32 SubpassIndex;

		struct FBlendAttachment
		{
			bool bBlend;
			uint8 ColorBlendOp;
			uint8 ColorBlendSrcFactor;
			uint8 ColorBlendDstFactor;
			uint8 AlphaBlendOp;
			uint8 AlphaBlendSrcFactor;
			uint8 AlphaBlendDstFactor;
			uint8 ColorWriteMask;

			void ReadFrom(const VkPipelineColorBlendAttachmentState& InState);
			void WriteInto(VkPipelineColorBlendAttachmentState& OutState) const;

			bool operator==(const FBlendAttachment& Other) const
			{
				return bBlend == Other.bBlend &&
					   ColorBlendOp == Other.ColorBlendOp &&
					   ColorBlendSrcFactor == Other.ColorBlendSrcFactor &&
					   ColorBlendDstFactor == Other.ColorBlendDstFactor &&
					   AlphaBlendOp == Other.AlphaBlendOp &&
					   AlphaBlendSrcFactor == Other.AlphaBlendSrcFactor &&
					   AlphaBlendDstFactor == Other.AlphaBlendDstFactor &&
					   ColorWriteMask == Other.ColorWriteMask;
			}
		};
		WEngine::WArray<FBlendAttachment> ColorBlendAttachmentStates;

		struct FVertexBinding
		{
			uint32 Stride;
			uint32 Binding;
			uint16 InputRate;

			void ReadFrom(const VkVertexInputBindingDescription& InState);
			void WriteInto(VkVertexInputBindingDescription& OutState) const;

			bool operator==(const FVertexBinding& Other) const
			{
				return Stride == Other.Stride &&
					   Binding == Other.Binding &&
					   InputRate == Other.InputRate;
			}
		};
		WEngine::WArray<FVertexBinding> VertexBindings;

		struct FVertexAttribute
		{
			uint32 Location;
			uint32 Binding;
			uint32 Format;
			uint32 Offset;

			void ReadFrom(const VkVertexInputAttributeDescription& InState);
			void WriteInto(VkVertexInputAttributeDescription& OutState) const;

			bool operator==(const FVertexAttribute& Other) const
			{
				return Location == Other.Location &&
					   Binding == Other.Binding &&
					   Format == Other.Format &&
					   Offset == Other.Offset;
			}
		};
		WEngine::WArray<FVertexAttribute> VertexAttributes;

		struct FRasterizer
		{
			uint8 PolygonMode;
			uint8 CullMode;
			float DepthBiasSlopeScale;
			float DepthBiasConstantFactor;

			void ReadFrom(const VkPipelineRasterizationStateCreateInfo& InState);
			void WriteInto(VkPipelineRasterizationStateCreateInfo& OutState) const;

			bool operator==(const FRasterizer& Other) const
			{
				return PolygonMode == Other.PolygonMode &&
					   CullMode == Other.CullMode &&
					   DepthBiasSlopeScale == Other.DepthBiasSlopeScale &&
					   DepthBiasConstantFactor == Other.DepthBiasConstantFactor;
			}
		};
		FRasterizer Rasterizer;

		struct FDepthStencil
		{
			uint8 DepthCompareOp;
			bool bDepthTestEnable;
			bool bDepthWriteEnable;
			bool bStencilTestEnable;
			bool bDepthBoundsTestEnable;
			uint8 FrontFailOp;
			uint8 FrontPassOp;
			uint8 FrontDepthFailOp;
			uint8 FrontCompareOp;
			uint32 FrontCompareMask;
			uint32 FrontWriteMask;
			uint32 FrontReference;
			uint8 BackFailOp;
			uint8 BackPassOp;
			uint8 BackDepthFailOp;
			uint8 BackCompareOp;
			uint32 BackCompareMask;
			uint32 BackWriteMask;
			uint32 BackReference;

			void ReadFrom(const VkPipelineDepthStencilStateCreateInfo& InState);
			void WriteInto(VkPipelineDepthStencilStateCreateInfo& OutState) const;

			bool operator==(const FDepthStencil& In) const
			{
				return DepthCompareOp == In.DepthCompareOp &&
					bDepthTestEnable == In.bDepthTestEnable &&
					bDepthWriteEnable == In.bDepthWriteEnable &&
					bDepthBoundsTestEnable == In.bDepthBoundsTestEnable &&
					bStencilTestEnable == In.bStencilTestEnable &&
					FrontFailOp == In.FrontFailOp &&
					FrontPassOp == In.FrontPassOp &&
					FrontDepthFailOp == In.FrontDepthFailOp &&
					FrontCompareOp == In.FrontCompareOp &&
					FrontCompareMask == In.FrontCompareMask &&
					FrontWriteMask == In.FrontWriteMask &&
					FrontReference == In.FrontReference &&
					BackFailOp == In.BackFailOp &&
					BackPassOp == In.BackPassOp &&
					BackDepthFailOp == In.BackDepthFailOp &&
					BackCompareOp == In.BackCompareOp &&
					BackCompareMask == In.BackCompareMask &&
					BackWriteMask == In.BackWriteMask &&
					BackReference == In.BackReference;
			}
		};
		FDepthStencil DepthStencil;

	};

	class VulkanGraphicsPipelineStateObject : public RHIPipelineStateObject
	{
	public:

		VulkanGraphicsPipelineStateObject(class VulkanDevice* pInDevice, const GfxPipelineDesc& InDesc);

		virtual ~VulkanGraphicsPipelineStateObject();

		virtual void Bind(class RHICommandBuffer *CmdBuffer) override;

		VkPipeline GetHandle() const { return Pipeline; }

	private:

		class VulkanDevice *pDevice;
		class VulkanRenderPass *RenderPass;
		class VulkanLayout *Layout;
		GfxPipelineDesc Desc;

		VkPipeline Pipeline;

		friend class VulkanPipelineStateManager;
	};

	class VulkanComputePipelineStateObject : public RHIPipelineStateObject
	{

	};

	class VulkanPipelineStateManager
	{
	public:

		VulkanPipelineStateManager(VulkanDevice* pInDevice)
			: pDevice(pInDevice)
		{
		}

		VulkanGraphicsPipelineStateObject* RHICreateGraphicsPipelineState(const RHIGraphicsPipelineStateInitializer& Initializer);

		VulkanGraphicsPipelineStateObject* GetGraphicsPipelineState(uint32 ID)
		{
			if (GraphicsPipelines.Find(ID))
			{
				return GraphicsPipelines[ID];
			}
			return nullptr;
		}

		void CreateGfxEntry(const RHIGraphicsPipelineStateInitializer& Initializer, class VulkanDescriptorSetLayout& DescriptorSetLayout, GfxPipelineDesc& Desc);

		bool CreateGfxPipelineFromtEntry(const RHIGraphicsPipelineStateInitializer& Initializer, VulkanGraphicsPipelineStateObject* PSO);

		void GetShaderModule(const RHIGraphicsPipelineStateInitializer& Initializer, VkShaderModule OutShaderModules[(uint8)EShaderStage::Count], const char* OutShaderEntries[(uint8)EShaderStage::Count]);

		VulkanComputePipelineStateObject* GetComputePipelineState(uint32 ID)
		{
			if (ComputePipelines.Find(ID))
			{
				return ComputePipelines[ID];
			}
			return nullptr;
		}

		void AddGraphicsPipelineState(uint32 ID, VulkanGraphicsPipelineStateObject* Pipeline)
		{
			GraphicsPipelines.Insert(ID, Pipeline);
		}

		void AddComputePipelineState(uint32 ID, VulkanComputePipelineStateObject* Pipeline)
		{
			ComputePipelines.Insert(ID, Pipeline);
		}

	private:

		VulkanDevice *pDevice;

		WEngine::WHashMap<uint32, VulkanGraphicsPipelineStateObject*> GraphicsPipelines;

		WEngine::WHashMap<uint32, VulkanComputePipelineStateObject*> ComputePipelines;

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