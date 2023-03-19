#include "pch.h"
#include "Platform/Vulkan/Public/VulkanVertexInputState.h"
#include "Render/Mesh/Public/Vertex.h"

/*
struct VertexInputElement
{
	uint32 SteamIndex = 0;
	uint32 Offset = 0;
	EVertexElementType Type = EVertexElementType::VET_Float1;
	uint32 AttribIndex = 0;
	uint32 Stride = 0;
	bool bUseInstance = false;
};
*/

namespace Vulkan
{

	VulkanVertexInputState::VulkanVertexInputState(const WEngine::WArray<VertexInputElement>& InElements)
		: RHIVertexInputState(InElements)
	{
		int32 LastStreamIndex = -1;
		for (VertexInputElement& Element : Elements)
		{
			if (Element.AttribIndex != LastStreamIndex)
			{
				NumVertexBindings++;
				LastStreamIndex = Element.SteamIndex;
			}


		}
	}

}