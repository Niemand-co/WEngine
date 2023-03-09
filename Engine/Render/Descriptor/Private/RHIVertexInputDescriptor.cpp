#include "pch.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Mesh/Public/Vertex.h"

void RHIVertexInputDescriptor::Generate(WEngine::WArray<class VertexInputElement>& Elements)
{
	for (VertexInputElement& Element : Elements)
	{
		VertexBindingDescription& Binding = BindingDescriptions[Element.AttribIndex];
		Binding.slot = Element.AttribIndex;
		Binding.stride = Element.Stride;

		VertexAttributeDescription& Attrib = AttributeDescriptions[Element.AttribIndex];
		Attrib.slot = Element.AttribIndex;
		Attrib.location
	}
}
