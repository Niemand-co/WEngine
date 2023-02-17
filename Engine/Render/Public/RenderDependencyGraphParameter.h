#pragma once
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"

struct WRDGRenderTargetBinding
{
	WRDGTexture* ColorTextures[MaxSimultaneousRenderTargets];
	WRDGTexture* DepthStencilTexture;
	WEngine::WResolveRect ResolveRect;
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

private:

	static const uint16 kInvalidOffset = WEngine::NumericLimits<uint16>::Max();

	uint16 RenderTargetOffset = kInvalidOffset;

	WEngine::WArray<ResourceInfo> Resources;

	WEngine::WArray<ResourceInfo> GraphTextures;

	WEngine::WArray<ResourceInfo> GraphBuffers;
};

class WRDGParameterStruct
{
public:

	WRDGParameterStruct(const uint8* inContents, const ShaderParametersLayout* inLayout)
		: Contents(inContents), Layout(inLayout)
	{
	}

	template<typename ParameterStructType>
	WRDGParameterStruct(const ParameterStructType* Parameters)
	{

	}

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

	ShaderParametersLayout* GetLayout() const { return Layout; }

private:

	void InitializeLayout();

private:

	WEngine::WString Name;

	int32 NumRows = 0;

	int32 NumColumns = 0;

	int32 NumElements = 0;

	ShaderParametersLayout *Layout;
	
};

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

#define BEGIN_SHADER_PARAMETERS_STRUCT(STRUCT_NAME)\
	__declspec(align(16)) struct STRUCT_NAME\
	{\
	private:\
		typedef STRUCT_NAME ThisStruct;
		typedef void* FuncPtr;\
		\
		template<uint32 FuncIndex>\
		static FuncPtr AppendMember(WEngine::WArray<WParameterMember>& Members)\
		{\
			return nullptr; \
		}\
		\
		template<>\
		static FuncPtr AppendMember<0>(WEngine::WArray<WParameterMember>& Members)\
		{\
			return nullptr;\
		}\
		typedef FuncPtr(*AppenMemberFunc)<0>(WEngine::WArray<WParameterMember>&);\
		typedef AppendMember

#define SHADER_PARAMETER_INTERNAL(MEMBER_TYPE, PARAMETER_TYPE, PARAMETER_NAME, DEFAULT_VALUE)\
	PreFuncPtr##PARAMETER_NAME;\
public:\
	WParameterTypeInfo<PARAMETER_TYPE>::AlignType PARAMETER_NAME DEFAULT_VALUE;\
private:\
	struct FuncPtrIdentity##PARAMETER_NAME {};\
	static FuncPtr AppendMember(FuncPtrIdentity##PARAMETER_NAME, WEngine::WArray<WParameterMember>& Members)\
	{\
		Members.Push(WParameterMember(#PARAMETER_NAME, MEMBER_TYPE, offsetof(ThisStruct, PARAMETER_NAME), WParameterTypeInfo<PARAMETER_TYPE>::NumRows, WParameterTypeInfo<PARAMETER_TYPE>::NumColumns, WParameterTypeInfo<PARAMETER_TYPE>::NumElements);\
		return PreFuncPtr##PARAMETER_NAME;\
	}\
	typedef FuncPtr(*NextFuncPtr##PARAMETER_NAME)(FuncPtrIdentity##PARAMETER_NAME, WEngine::WArray<WParameterMember>& Members);\
	typedef NextFuncPtr##PARAMETER_NAME

#define END_SHADER_PARAMETERS_STRUCT\
	FuncPtr_HEAD;\
		static WEngine::WArray<WParameterMember> AddMember(const WEngine::WString& InName, EUniformBaseType InBaseType, uint32 InOffset, uint32 InNumRows, uint32 InNumColumns, uint32 InNumElements)\
		{\
			WEngine::WArray<WParameterMember> Members;\
			FuncPtr ptr = FuncPtr_HEAD;\
			do\
			{\
				reinterpret_cast<AppendMember>(ptr)();\
			}while(ptr);\
			return Members; \
		}; \
	};

#define RENDER_TARGET_SLOTS\
	WRDGRenderTargetBinding *RenderTarget = nullptr;

#define SHADER_PARAMETER(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(WParameterTypeInfo<PARAMETER_TYPE>::BaseType, PARAMETER_TYPE, PARAMETER_NAME, )

#define SHADER_PARAMETER_SAMPLER(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_SAMPLER, PARAMETER_TYPE, PARAMETER_NAME, = nullptr)

#define SHADER_PARAMETER_TEXTURE(PARAMETER_TYPE, PARAMETER_NAME)\
	SHADER_PARAMETER_INTERNAL(EUniformBaseType::UB_TEXTURE, PARAMETER_TYPE, PARAMETER_NAME, = nullptr)