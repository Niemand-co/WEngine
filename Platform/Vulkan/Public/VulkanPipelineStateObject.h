#pragma once
#include "RHI/Public/RHIPipelineStateObject.h"

namespace Vulkan
{

	struct GfxPipelineDesc
	{
		uint16 RasterizationSamples;
		uint32 Topology;
		uint8 UseAlphaToCoverage;
		uint8 SubpassIndex;

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

		struct FRenderTargets
		{
			struct FAttachmentReference
			{
				uint32 AttachmentIndex;
				uint64 Layout;

				void ReadFrom(const VkAttachmentReference& InState);
				void WriteInto(VkAttachmentReference& OutState) const;
				bool operator==(const FAttachmentReference& Other) const
				{
					return AttachmentIndex == Other.AttachmentIndex && Layout == Other.Layout;
				}
			};
			WEngine::WArray<FAttachmentReference> ColorAttachments;
			WEngine::WArray<FAttachmentReference> ResolveAttachments;
			FAttachmentReference DepthStencil;

			struct FAttachment
			{
				uint32 Format;
				uint8 Flags;
				uint8 Samples;
				uint8 LoadOp;
				uint8 StoreOp;
				uint8 StencilLoadOp;
				uint8 StencilStoreOp;
				uint64 InitialLayout;
				uint64 FinalLayout;

				void ReadFrom(const VkAttachmentDescription& InState);
				void WriteInto(VkAttachmentDescription& OutState) const;
				bool operator==(const FAttachment& Other) const
				{
					return Format == Other.Format &&
						   Flags == Other.Flags &&
						   Samples == Other.Samples &&
						   LoadOp == Other.LoadOp &&
						   StoreOp == Other.StoreOp &&
						   StencilLoadOp == Other.StencilStoreOp &&
						   StencilStoreOp == Other.StencilStoreOp &&
						   InitialLayout == Other.InitialLayout &&
						   FinalLayout == Other.FinalLayout;
				}
			};
			WEngine::WArray<FAttachment> Attachments;

			uint8 NumAttachments;
			uint8 NumColorAttachments;
			uint8 bHasDepthStencil;
			uint8 bHasResolveAttachments;
			uint8 NumUsedClearValues;
			uint32 RenderPassHash;
			glm::vec3 Extent3D;

			void ReadFrom(const VulkanRenderTargetLayout& InState);
			void WriteInto(VulkanRenderTargetLayout& OutState) const;
			bool operator==(const FRenderTargets& Other) const
			{
				return ColorAttachments == Other.ColorAttachments &&
					   ResolveAttachments == Other.ResolveAttachments &&
					   DepthStencil == Other.DepthStencil &&
					   Attachments == Other.Attachments &&
					   NumAttachments == Other.NumAttachments &&
					   NumColorAttachments == Other.NumColorAttachments &&
					   bHasDepthStencil == Other.bHasDepthStencil &&
					   bHasResolveAttachments == Other.bHasResolveAttachments &&
					   NumUsedClearValues == Other.NumUsedClearValues &&
					   Extent3D == Other.Extent3D;
			}
		};
		FRenderTargets RenderTargets;

		bool operator==(const GfxPipelineDesc& Other) const
		{
			return RasterizationSamples == Other.RasterizationSamples &&
				   Topology == Other.Topology &&
				   UseAlphaToCoverage == Other.UseAlphaToCoverage &&
				   SubpassIndex == Other.SubpassIndex &&
				   ColorBlendAttachmentStates == Other.ColorBlendAttachmentStates &&
				   Rasterizer == Other.Rasterizer &&
				   DepthStencil == Other.DepthStencil &&
				   VertexBindings == Other.VertexBindings &&
				   VertexAttributes == Other.VertexAttributes &&
				   RenderTargets == Other.RenderTargets;
		}

		uint32 GetKey() const;
	};

	class VulkanGraphicsPipelineState : public RHIGraphicsPipelineState
	{
	public:

		VulkanGraphicsPipelineState(class VulkanDevice* pInDevice, const RHIGraphicsPipelineStateInitializer& Initializer, const GfxPipelineDesc& InDesc);

		virtual ~VulkanGraphicsPipelineState();

		virtual void Bind(RHICommandBuffer *CmdBuffer) override;

		const class VulkanVertexInputState* GetVertexInputState() const { return pVertexInputState; }

		VkPipeline GetHandle() const { return Pipeline; }

	private:

		class VulkanDevice *pDevice;
		class VulkanVertexInputState *pVertexInputState;
		class VulkanRenderPass *RenderPass;
		class VulkanLayout *Layout;
		GfxPipelineDesc Desc;

		class VulkanShaderBase* VulkanShaders[(uint8)EShaderStage::Count];

		VkPipeline Pipeline;

		friend class VulkanPipelineStateManager;
	};

	class VulkanComputePipelineState : public RHIComputePipelineState
	{

	};

	class VulkanPipelineStateManager
	{
	public:

		VulkanPipelineStateManager(VulkanDevice* pInDevice)
			: pDevice(pInDevice)
		{
		}

		VulkanGraphicsPipelineState* RHICreateGraphicsPipelineState(const RHIGraphicsPipelineStateInitializer& Initializer);

		VulkanLayout* FindOrAddLayout(const VulkanDescriptorSetLayout& DescriptorSetLayout);

		void CreateGfxEntry(const RHIGraphicsPipelineStateInitializer& Initializer, class VulkanDescriptorSetLayout& DescriptorSetLayout, GfxPipelineDesc& Desc);

		bool CreateGfxPipelineFromtEntry(const RHIGraphicsPipelineStateInitializer& Initializer, VulkanGraphicsPipelineState* PSO);

		void GetShaderModule(const RHIGraphicsPipelineStateInitializer& Initializer, VkShaderModule OutShaderModules[(uint8)EShaderStage::Count], const char* OutShaderEntries[(uint8)EShaderStage::Count]);

	private:

		VulkanDevice *pDevice;

		WCriticalSection GraphicsPSOLock;

		WCriticalSection LayoutLock;

		WEngine::WHashMap<uint32, VulkanGraphicsPipelineState*> GraphicsPipelines;

		WEngine::WHashMap<uint32, VulkanLayout*> Layouts;

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