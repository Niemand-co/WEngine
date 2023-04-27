#pragma once

namespace Vulkan
{
	
	struct UniformBufferGatherInfo
	{
		WEngine::WHashMap<uint32, VkShaderStageFlags> LayoutsToStageMap;

		const class VulkanShaderCodeHeader *CodeHeaders[(uint8)EShaderStage::Count];
	};

	class VulkanDescriptorSetLayout : public VulkanResource
	{
	public:

		VulkanDescriptorSetLayout();

		virtual ~VulkanDescriptorSetLayout();

		struct SetLayout
		{
			WEngine::WArray<VkDescriptorSetLayoutBinding> Bindings;
			uint32 Hash;

			inline void GenerateHash()
			{
				Hash = WEngine::MemCrc32(Bindings.GetData(), sizeof(VkDescriptorSetLayoutBinding) * Bindings.Size());
			}

			uint32 GetHash() const
			{
				return Hash;
			}

			inline bool operator==(const SetLayout& Other) const
			{
				if (Hash != Other.Hash)
				{
					return false;
				}

				if (Bindings.Size() != Other.Bindings.Size())
				{
					return false;
				}

				if (Bindings.Size() > 0 && memcmp(Bindings.GetData(), Other.Bindings.GetData(), sizeof(VkDescriptorSetLayoutBinding) * Bindings.Size()) == 0)
				{
					return false;
				}

				return false;
			}

			inline bool operator!=(const SetLayout& Other) const
			{
				return !(*this == Other);
			}
		};

		VkDescriptorSetLayout GetHandle() const { return DescriptorSetLayout; }

		WEngine::WArray<SetLayout>& GetLayouts() { return Layouts; }

		uint32 GetTypeHash() const { return Hash; }

		void ProcessBindingForStage(VkShaderStageFlags ShaderStage, UniformBufferGatherInfo& OutGatherInfo, const class VulkanShaderCodeHeader& CodeHeader);

		void FinalizeBindings(const VulkanDevice *Device, const UniformBufferGatherInfo& GatherInfo, const WEngine::WArray<RHISamplerState*>& InnutableSamplers);

	private:

		VulkanDevice* pDevice;

		VkDescriptorSetLayout DescriptorSetLayout;

		WEngine::WArray<SetLayout> Layouts;

		uint32 Hash;

	};

	class VulkanDescriptorSet : public VulkanResource
	{
	public:

		VulkanDescriptorSet(class VulkanDevice *pInDevice);

		virtual ~VulkanDescriptorSet();

		VkDescriptorSet GetHandle() const { return DescriptorSet; }

	private:

		VulkanDevice *pDevice;

		VkDescriptorSet DescriptorSet;

	};

	class VulkanDescriptorSetLayoutManager : public VulkanResource
	{
	public:

		static VulkanDescriptorSetLayout* GetDescriptorSetLayout(uint32 ID)
		{
			if (Layouts.Find(ID))
			{
				return Layouts[ID];
			}
			return nullptr;
		}

		static void AddDescriptorLayout(uint32 ID, VulkanDescriptorSetLayout* Layout)
		{
			Layouts[ID] = Layout;
		}

	private:

		static WEngine::WHashMap<uint32, VulkanDescriptorSetLayout*> Layouts;

	};

}