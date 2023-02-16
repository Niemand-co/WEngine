#pragma once
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"

struct WRDGRenderTargetBinding
{
	WRDGTexture* ColorTextures[MaxSimultaneousRenderTargets];
	WRDGTexture* DepthStencilTexture;
	WEngine::WResolveRect ResolveRect;
};

class ShaderParametersLayout
{
public:

	struct ResourceInfo
	{
		uint16 Offset;
		EUniformBaseType Type;
	};

	ShaderParametersLayout();

	uint16 GetRenderTargetOffset() const { return RenderTargetOffset; }

private:

	static const uint16 kInvalidOffset = WEngine::NumericLimits<uint16>::Max;

	uint16 RenderTargetOffset = kInvalidOffset;

	WEngine::WArray<ResourceInfo> Resources;

	WEngine::WArray<ResourceInfo> GraphTextures;

	WEngine::WArray<ResourceInfo> GraphBuffers;
};

class WRDGParameterStruct
{
public:

	WRDGParameterStruct();

	~WRDGParameterStruct();

	void InitializeLayout();

	RHIRenderPassDescriptor GetRenderPassInfo() const;

	bool HasRenderTargets() const;

	bool HasExternalOutput() const;

	const WRDGRenderTargetBinding& GetRenderTarget() const
	{
		return *reinterpret_cast<const WRDGRenderTargetBinding*>(Contents + Layout->GetRenderTargetOffset());
	}

private:

	const uint8* Contents;

	const ShaderParametersLayout *Layout;

};

struct WShaderParameterMetaData
{
public:

	WEngine::WString GetName() const { return Name; }

	int32 GetRowsNum() const { return NumRows; }

	int32 GetColumnsNum() const { return NumColumns; }

	int32 GetElementNum() const { return NumElements; }

private:

	WEngine::WString Name;

	int32 NumRows = 0;

	int32 NumColumns = 0;

	int32 NumElements = 0;
	
};

template<typename ParameterType>
struct WParameterTypeInfo
{
	typedef ParameterType Type;

	static constexpr int32 Alignment = alignof(ParameterType);

	static constexpr int32 NumElements = 0;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return ParameterType::StructMetaData; }
};

#define BEGIN_SHADER_PARAMETERS_STRUCT(STRUCT_NAME)\
	struct STRUCT_NAME\
	{\
		static WShaderParameterMetaData StructMetaData(##STRUCT_NAME);

#define END_SHADER_PARAMETERS_STRUCT };

#define RENDER_TARGET_SLOTS\
	WRDGRenderTargetBinding *RenderTarget = nullptr;

#define SHADER_PARAMETER(PARAMETER_TYPE, PARAMETER_NAME)\
	PARAMETER_TYPE PARAMETER_NAME;