#pragma once
#include "Render/Public/RenderDependencyGraphDefinitions.h"
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"

inline EAccess GetPassAccess(EPassFlag Flag, EAccess& SRVAccess, EAccess& UAVAccess);

struct WRDGRenderTargetBinding
{
	WRDGTexture* ColorTextures[MaxSimultaneousRenderTargets];
	WRDGTexture* DepthStencilTexture;
	WEngine::WResolveRect ResolveRect;

	bool operator==(const WRDGRenderTargetBinding& other) const
	{
		for (uint32 ColorIndex = 0; ColorIndex < MaxSimultaneousRenderTargets; ++ColorIndex)
		{
			if (ColorTextures[ColorIndex] != other.ColorTextures[ColorIndex])
			{
				return false;
			}
		}

		if (DepthStencilTexture != other.DepthStencilTexture)
		{
			return false;
		}

		if (ResolveRect == other.ResolveRect)
		{
			return false;
		}

		return true;
	}
};

struct WParameterMember
{
	WParameterMember(const WEngine::WString& InName, EUniformBaseType InBaseType, uint32 InOffset, uint32 InNumRows, uint32 InNumColumns, uint32 InNumElements)
		: Name(InName),
		  BaseType(InBaseType),
		  Offset(InOffset),
		  NumRows(InNumRows),
		  NumColumns(InNumColumns),
		  NumElements(InNumElements)
	{
	}

	WEngine::WString Name;
	EUniformBaseType BaseType;
	uint32 Offset;
	uint32 NumRows;
	uint32 NumColumns;
	uint32 NumElements;
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

	bool HasRenderTarget() const { return RenderTargetOffset != kInvalidOffset; }

private:

	static const uint16 kInvalidOffset = WEngine::NumericLimits<uint16>::Max();

	uint16 RenderTargetOffset = kInvalidOffset;

	WEngine::WArray<ResourceInfo> Resources;

	WEngine::WArray<ResourceInfo> GraphResources;

	WEngine::WArray<ResourceInfo> GraphTextures;

	WEngine::WArray<ResourceInfo> GraphBuffers;

	WEngine::WArray<ResourceInfo> UniformBuffers;

	friend struct WShaderParameterMetaData;
};

struct WShaderParameterMetaData
{
public:

	WShaderParameterMetaData(const char* InName, const WEngine::WArray<WParameterMember>& InMembers)
		: Name(InName),
		  Members(InMembers)
	{
		InitializeLayout();
	}

	WEngine::WString GetName() const { return Name; }

	const ShaderParametersLayout& GetLayout() const { return Layout; }

private:

	void InitializeLayout();

private:

	WEngine::WString Name;

	ShaderParametersLayout Layout;

	WEngine::WArray<WParameterMember> Members;

};

class WRDGParameterStruct
{
public:

	WRDGParameterStruct(const uint8* inContents, const ShaderParametersLayout* inLayout)
		: Contents(inContents), Layout(inLayout)
	{
		const WRDGRenderTargetBinding& RenderTarget = *reinterpret_cast<const WRDGRenderTargetBinding*>(Contents + Layout->GetRenderTargetOffset());
		for (uint32 ColorAttachmentIndex = 0; ColorAttachmentIndex < MaxSimultaneousRenderTargets; ++ColorAttachmentIndex)
		{
			if (RenderTarget.ColorTextures[ColorAttachmentIndex] != nullptr)
			{
				WRDGTexture* Texture = RenderTarget.ColorTextures[ColorAttachmentIndex];
				if (Texture->IsExternal())
				{
					bHasExternalOutput = true;
					return;
				}
			}
		}
		if(RenderTarget.DepthStencilTexture->IsExternal())
			bHasExternalOutput = true;
		bHasExternalOutput = false;
	}

	template<typename ParameterStructType>
	WRDGParameterStruct(const ParameterStructType* Parameters)
		: WRDGParameterStruct(reinterpret_cast<const uint8*>(Parameters), &Parameters->GetStructMetaData()->GetLayout())
	{
	}

	~WRDGParameterStruct() = default;

	RHIRenderPassDescriptor GetRenderPassInfo() const;

	bool HasRenderTargets() const { Layout->HasRenderTarget(); }

	bool HasExternalOutput() const;

	const WRDGRenderTargetBinding& GetRenderTarget() const
	{
		return *reinterpret_cast<const WRDGRenderTargetBinding*>(Contents + Layout->GetRenderTargetOffset());
	}

	const uint8* GetContents() const { return Contents; }

	const ShaderParametersLayout* GetLayout() const { return Layout; }

	template<typename LAMBDA>
	void EnumerateTextures(EPassFlag PassFlag, LAMBDA lambda) const;

	template<typename LAMBDA>
	void EnumerateBuffers(EPassFlag PassFlag, LAMBDA lambda) const;

private:

	const uint8* Contents;

	const ShaderParametersLayout *Layout;

	uint8 bHasExternalOutput : 1;

};

template<typename LAMBDA>
inline void WRDGParameterStruct::EnumerateTextures(EPassFlag PassFlag, LAMBDA lambda) const
{
	for (uint32 Index; Index < Layout->GraphTextures.Size(); ++Index)
	{
		EAccess SRVAccess = EAccess::Unknown;
		EAccess UAVAccess = EAccess::Unknown;
		GetPassAccess(PassFlag, SRVAccess, UAVAccess);

		switch (Layout->GraphTextures[Index].Type)
		{
		case EUniformBaseType::UB_RDG_TEXTURE:
			lambda(Layout->GraphTextures[Index].Type, (WRDGTexture*)(Contents + Layout->GraphTextures[Index].Offset), SRVAccess);
			break;
		case EUniformBaseType::UB_RDG_TEXTURE_SRV:
			lambda(Layout->GraphTextures[Index].Type, (WRDGTextureSRV*)(Contents + Layout->GraphTextures[Index].Offset), SRVAccess);
			break;
		case EUniformBaseType::UB_RDG_BUFFER_UAV:
		{
			lambda(Layout->GraphTextures[Index].Type, (WRDGTextureUAV*)(Contents + Layout->GraphTextures[Index].Offset), UAVAccess);
			break;
		}
		case EUniformBaseType::UB_RTV:
		{
			WRDGRenderTargetBinding*  RenderTarget = (WRDGRenderTargetBinding*)(Contents + Layout->GraphTextures[Index].Offset);
			for (uint32 ColorIndex = 0; ColorIndex < MaxSimultaneousRenderTargets; ++ColorIndex)
			{
				lambda(Layout->GraphTextures[Index].Type, RenderTarget->ColorTextures[ColorIndex], EAccess::RTV);
			}
			if (RenderTarget->DepthStencilTexture)
			{
				lambda(Layout->GraphTextures[Index].Type, RenderTarget->DepthStencilTexture, EAccess::RTV)
			}
			break;
		}
		default:
			break;
		}
	}
}

template<typename LAMBDA>
inline void WRDGParameterStruct::EnumerateBuffers(EPassFlag PassFlag, LAMBDA lambda) const
{
	for (uint32 Index; Index < Layout->GraphBuffers.Size(); ++Index)
	{
		EAccess SRVAccess = EAccess::Unknown;
		EAccess UAVAccess = EAccess::Unknown;
		GetPassAccess(PassFlag, SRVAccess, UAVAccess);

		switch (Layout->GraphBuffers[Index].Type)
		{
		case EUniformBaseType::UB_RDG_BUFFER:
			lambda(Layout->GraphBuffers[Index].Type, (WRDGBuffer*)(Contents + Layout->GraphBuffers[Index].Offset), SRVAccess);
			break;	
		case EUniformBaseType::UB_RDG_BUFFER_SRV:
			lambda(Layout->GraphBuffers[Index].Type, (WRDGBuffer*)(Contents + Layout->GraphBuffers[Index].Offset), SRVAccess);
			break;
		case EUniformBaseType::UB_RDG_BUFFER_UAV:
			lambda(Layout->GraphBuffers[Index].Type, (WRDGBuffer*)(Contents + Layout->GraphBuffers[Index].Offset), UAVAccess);
			break;
		default:
			break;
		}
	}
}

template<typename ParameterType>
struct WParameterTypeInfo
{
	typedef ParameterType Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_INVALID;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 1;
	static constexpr int32 Alignment = alignof(ParameterType);

	typedef __declspec(align(Alignment)) ParameterType AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return ParameterType::StructMetaData; }
};

template<>
struct WParameterTypeInfo<bool>
{
	typedef bool Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_BOOL;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 1;
	static constexpr int32 Alignment = 4;

	typedef __declspec(align(4)) bool AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<int32>
{
	typedef int32 Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_INT32;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 1;
	static constexpr int32 Alignment = 4;

	typedef __declspec(align(4)) int32 AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<uint32>
{
	typedef uint32 Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_UINT32;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 1;
	static constexpr int32 Alignment = 4;

	typedef __declspec(align(4)) uint32 AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<float>
{
	typedef float Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_FLOAT32;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 1;
	static constexpr int32 Alignment = 4;

	typedef __declspec(align(4)) float AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<glm::vec2>
{
	typedef glm::vec2 Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_FLOAT32;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 2;
	static constexpr int32 Alignment = 8;

	typedef __declspec(align(8)) glm::vec2 AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<glm::vec3>
{
	typedef glm::vec3 Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_FLOAT32;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 3;
	static constexpr int32 Alignment = 16;

	typedef __declspec(align(16)) glm::vec3 AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<glm::vec4>
{
	typedef glm::vec4 Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_FLOAT32;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 4;
	static constexpr int32 Alignment = 16;

	typedef __declspec(align(16)) glm::vec4 AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<glm::mat4>
{
	typedef glm::mat4 Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_FLOAT32;

	static constexpr int32 NumElements = 0;
	static constexpr int32 NumRows = 3;
	static constexpr int32 NumColumns = 3;
	static constexpr int32 Alignment = 16;

	typedef __declspec(align(16)) glm::mat4 AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

template<>
struct WParameterTypeInfo<WRDGRenderTargetBinding*>
{
	typedef WRDGRenderTargetBinding* Type;

	static constexpr EUniformBaseType BaseType = EUniformBaseType::UB_SRV;

	static constexpr int32 NumElements = MaxSimultaneousRenderTargets + 1;
	static constexpr int32 NumRows = 1;
	static constexpr int32 NumColumns = 1;
	static constexpr int32 Alignment = 8;

	typedef __declspec(align(8)) WRDGRenderTargetBinding* AlignType;

	static constexpr WShaderParameterMetaData* GetStructMetaData() { return nullptr; }
};

#define BEGIN_SHADER_PARAMETERS_STRUCT(STRUCT_NAME)\
	__declspec(align(16)) struct STRUCT_NAME\
	{\
	public:\
		STRUCT_NAME() = default;\
		static WShaderParameterMetaData* GetStructMetaData()\
		{\
			static WShaderParameterMetaData MetaData(#STRUCT_NAME, GetMembers());\
			return &MetaData;\
		}\
	private:\
		typedef STRUCT_NAME ThisStruct;\
		typedef void* FuncPtr;\
		struct FuncPtrIdentity {};\
		static FuncPtr AppendMember(FuncPtrIdentity, WEngine::WArray<WParameterMember>& Members)\
		{\
			return nullptr; \
		}\
		typedef FuncPtr(*AppendMemberType)(FuncPtrIdentity, WEngine::WArray<WParameterMember>&);\
		typedef FuncPtrIdentity

#define SHADER_PARAMETER_INTERNAL(MEMBER_TYPE, PARAMETER_TYPE, PARAMETER_NAME, DEFAULT_VALUE)\
	PreFuncPtrIdentity##PARAMETER_NAME;\
public:\
	WParameterTypeInfo<PARAMETER_TYPE>::AlignType PARAMETER_NAME DEFAULT_VALUE;\
private:\
	struct FuncPtrIdentity##PARAMETER_NAME {};\
	static FuncPtr AppendMember(FuncPtrIdentity##PARAMETER_NAME, WEngine::WArray<WParameterMember>& Members)\
	{\
		Members.Push(WParameterMember(#PARAMETER_NAME, MEMBER_TYPE, offsetof(ThisStruct, PARAMETER_NAME), WParameterTypeInfo<PARAMETER_TYPE>::NumRows, WParameterTypeInfo<PARAMETER_TYPE>::NumColumns, WParameterTypeInfo<PARAMETER_TYPE>::NumElements));\
		FuncPtr(*Ptr)(PreFuncPtrIdentity##PARAMETER_NAME, WEngine::WArray<WParameterMember>&);\
		Ptr = AppendMember;\
		return (FuncPtr)Ptr;\
	}\
	typedef FuncPtrIdentity##PARAMETER_NAME

#define END_SHADER_PARAMETERS_STRUCT\
	LastFuncPtrIdentity;\
		static WEngine::WArray<WParameterMember> GetMembers()\
		{\
			WEngine::WArray<WParameterMember> Members;\
			FuncPtr(*LastPtr)(LastFuncPtrIdentity, WEngine::WArray<WParameterMember>&);\
			LastPtr = AppendMember;\
			FuncPtr ptr = (FuncPtr)LastPtr;\
			do\
			{\
				ptr = reinterpret_cast<AppendMemberType>(ptr)(FuncPtrIdentity(), Members);\
			}while(ptr);\
			return Members; \
		}; \
	};

#define RENDER_TARGET_SLOTS()\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_RTV, WRDGRenderTargetBinding*, RenderTarget, = nullptr)

#define SHADER_PARAMETER(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(WParameterTypeInfo<PARAMETER_TYPE>::BaseType, PARAMETER_TYPE, PARAMETER_NAME, )

#define SHADER_PARAMETER_SAMPLER(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_SAMPLER, PARAMETER_TYPE, PARAMETER_NAME, = nullptr)

#define SHADER_PARAMETER_TEXTURE(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_TEXTURE, PARAMETER_TYPE, PARAMETER_NAME, = nullptr)

#define SHADER_PARAMETER_SRV(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_RDG_TEXTURE_SRV, PARAMETER_TYPE, PARAMETER_NAME, = nullptr)

#define SHADER_PARAMETER_UAV(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_RDG_TEXTURE_UAV, PARAMETER_TYPE, PARAMETER_NAME, = nullptr)

#define SHADER_PARAMETER_RDG_TEXTURE(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_RDG_TEXTURE, PARAMETER_TYPE, PARAMETER_NAME, = nullptr)

BEGIN_SHADER_PARAMETERS_STRUCT(DeferredBasePassParameters)
	SHADER_PARAMETER_RDG_TEXTURE(WRDGTexture*, GBuffer0)
	SHADER_PARAMETER_RDG_TEXTURE(WRDGTexture*, GBuffer1)
	SHADER_PARAMETER_RDG_TEXTURE(WRDGTexture*, GBuffer2)
	SHADER_PARAMETER_RDG_TEXTURE(WRDGTexture*, GBuffer3)
	RENDER_TARGET_SLOTS()
END_SHADER_PARAMETERS_STRUCT