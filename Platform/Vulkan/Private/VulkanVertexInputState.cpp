#include "pch.h"
#include "Platform/Vulkan/Public/VulkanVertexInputState.h"
#include "Render/Mesh/Public/Vertex.h"

namespace Vulkan
{

	VulkanVertexInputState::VulkanVertexInputState(const WEngine::WArray<VertexInputElement>& InElements)
		: RHIVertexInputState(InElements)
	{
		uint32 StreamIndexBitMask = 0;
		for (const VertexInputElement& Element : Elements)
		{
			VkVertexInputBindingDescription& Binding = VertexInputBindings[Element.StreamIndex];
			if (((1 << Element.StreamIndex) & StreamIndexBitMask) != 0)
			{
				RE_ASSERT(Binding.binding == Element.StreamIndex, "Vertex binding is occupied by multi elements.");
				RE_ASSERT(Binding.inputRate == Element.bUseInstance ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX, "Vertex binding is occupied by multi elements.");
				RE_ASSERT(Binding.stride == Element.Stride, "Vertex binding is occupied by multi elements.");
			}
			else
			{
				Binding.binding = Element.StreamIndex;
				Binding.inputRate = Element.bUseInstance ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
				Binding.stride = Element.Stride;

				StreamIndexBitMask |= (1 << Element.StreamIndex);
			}
		}

		for (uint32 BindingIndex = 0; BindingIndex < MaxVertexInputElementCount; ++BindingIndex)
		{
			if ((StreamIndexBitMask & (1 << BindingIndex)) != 0)
			{
				BindingToStream[BindingIndex] = Bindings.Size();
				VertexInputAttributes[BindingIndex].binding = Bindings.Size();
				Bindings.Push(VertexInputBindings[BindingIndex]);
			}
		}

		uint32 NumAttributes = 0;
		for (const VertexInputElement& Element : Elements)
		{
			if(!(1 << Element.AttribIndex) || !BindingToStream.Find(Element.StreamIndex))
				continue;

			VkVertexInputAttributeDescription& Attrib = VertexInputAttributes[NumAttributes++];
			Attrib.binding = BindingToStream[Element.StreamIndex];
			Attrib.location = Element.AttribIndex;
			Attrib.offset = Element.Offset;
			Attrib.format = WEngine::ToVulkan(Element.Type);
		}

		VertexInputStateCreateInfo = {};
		VertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		VertexInputStateCreateInfo.vertexBindingDescriptionCount = Bindings.Size();
		VertexInputStateCreateInfo.pVertexBindingDescriptions = Bindings.GetData();
		VertexInputStateCreateInfo.vertexAttributeDescriptionCount = NumAttributes;
		VertexInputStateCreateInfo.pVertexAttributeDescriptions = VertexInputAttributes;
	}

	WEngine::WHashMap<uint32, VulkanVertexInputState*> VulkanVertexInputStateManager::VertexInputs = WEngine::WHashMap<uint32, VulkanVertexInputState*>();

}