#pragma once
#include "RHI/Public/RHIBuffer.h"

template<typename TBufferStruct>
class TUniformBufferRef : public FUniformBufferRHIRef
{
public:

	static TUniformBufferRef<TBufferStruct> CreateUniformBufferImmediate(const TBufferStruct& Value, EUniformBufferUsage Usage)
	{
		return TUniformBufferRef<TBufferStruct>(GetRenderCommandList()->CreateUniformBuffer((uint8*)&Value, &TBufferStruct::FTypeInfo::GetStructMetaData()->GetLayout(), Usage));
	}

	void UpdateUniformBufferImmediate(const TBufferStruct& Value)
	{
		GetRenderCommandList()->UpdateUniformBuffer(Get(), &Value);
	}

private:

	TUniformBufferRef(WUniformBufferRHIRef InRHIRef)
		: FUniformBufferRHIRef(InRHIRef)
	{
	}

};