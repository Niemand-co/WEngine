#pragma once
#include "Render/Public/RenderDependencyGraphParameter.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"

template<typename InObjectType, typename IndexType>
class WRDGHandle
{
public:

	typedef InObjectType ObjectType;

	WRDGHandle() = default;

	explicit WRDGHandle(IndexType inIndex)
		: Index(inIndex)
	{
	}

	~WRDGHandle() = default;

	bool IsValid() { return Index != NullIndex; }
	bool operator==(const WRDGHandle& other) { RE_ASSERT(IsValid, "Handle Invalid."); return Index == other.Index; }
	bool operator!=(const WRDGHandle& other) { RE_ASSERT(IsValid, "Handle Invalid."); return Index != other.Index; }
	bool operator<=(const WRDGHandle& other) { RE_ASSERT(IsValid, "Handle Invalid."); return Index <= other.Index; }
	bool operator>=(const WRDGHandle& other) { RE_ASSERT(IsValid, "Handle Invalid."); return Index >= other.Index; }
	bool operator<(const WRDGHandle& other) { RE_ASSERT(IsValid, "Handle Invalid."); return Index < other.Index; }
	bool operator>(const WRDGHandle& other) { RE_ASSERT(IsValid, "Handle Invalid."); return Index > other.Index; }

	IndexType operator-(const WRDGHandle& other)
	{
		return Index - other.Index;
	}

	WRDGHandle& operator++()
	{
		Index++;
		return *this;
	}

	WRDGHandle& operator--()
	{
		Index--;
		return *this;
	}

public:

	static constexpr IndexType NullIndex = WEngine::NumericLimits<IndexType>::Max;

	IndexType Index = NullIndex;

};

template<typename HandleType>
class WRDGHandleBitArray : public WEngine::WBitArray
{
public:

	WRDGHandleBitArray() = default;

	WRDGHandleBitArray(uint32 Count, bool Value)
		: WEngine::WBitArray(Count, Value)
	{
	}

	WBitReference operator[](HandleType Handle)
	{
		return WEngine::WBitArray::operator[](Handle.Index);
	}

	WConstBitReference operator[](HandleType Handle) const
	{
		return WENgine::WBitArray::operator[](Handle.Index);
	}

};

template<typename HandleType>
class WRDGHandleRegistry
{
public:

	typedef HandleType::ObjectType ObjectType;

	WRDGHandleRegistry() = default;

	~WRDGHandleRegistry() = default;

	void Insert(ObjectType *Object)
	{
		Object->Handle = Array.Size();
		Array.Push(Object);
	}

	template<typename AllocObjectType, typename... Args>
	AllocObjectType* Allocate(Args... args)
	{
		AllocObjectType *object = (AllocObjectType*)WRDGAllocator::Get()->Allocate(sizeof(AllocObjectType));
		::new (object) Object(args...);
		Insert(static_cast<ObjectType*>(object));
		return object;
	}

	ObjectType* operator[](HandleType handle)
	{
		return Array[handle.Index];
	}

	const ObjectType* operator[](HandleType handle) const
	{
		return Array[handle.Index];
	}

	HandleType Begin() const
	{
		return HandleType(0);
	}

	HandleType End() const
	{
		return HandleType(Array.Size());
	}

private:

	WEngine::WArray<ObjectType*> Array;

};

class WRDGBarrierBatch : public RHIBarrierBatch
{
public:

	WRDGBarrierBatch() = default;

	virtual ~WRDGBarrierBatch() = default;

	void Submit(RHIRenderCommandList& CmdList);

private:

	WEngine::WArray<class RHIBarrierDescriptor> BarrierDesc;

};

class WRDGPass
{
public:

	WRDGPass(WEngine::WString&& inName,  WRDGParameterStruct inParameters)
		: Name(inName),
		  Parameters(inParameters)
	{
	}

	virtual ~WRDGPass() = default;

	virtual void Execute() = 0;

	void SetNeverCulling(bool bInCulling) { Flag = (EPassFlag)((uint16)Flag | (uint16)EPassFlag::NeverCull); }

	bool IsNeverCulling() const { return ((uint16)Flag & (uint16)EPassFlag::NeverCull) != 0; }

	EPipeline GetPipeline() const { if(Flag == EPassFlag::AsyncCompute) return EPipeline::AsyncCompute; else return EPipeline::Graphics; }

private:

	struct TextureState
	{
		WEngine::WArray<WRDGResourceState> States;
		WEngine::WArray<WRDGResourceState*> MergeStates;
		uint32 ReferenceCount;
	};

	struct BufferState
	{
		WRDGResourceState State;
		WRDGResourceState* MergeState;
		uint32 ReferenceCount;
	};

	WRDGPassHandle Handle;

	WEngine::WString Name;

	const WRDGParameterStruct Parameters;

	WRDGPassHandleArray Producers;

	WEngine::WHashMap<class WRDGBuffer*, BufferState> BufferStates;

	WEngine::WHashMap<class WRDGTexture*, TextureState> TextureStates;

	WEngine::WArray<class RHITexture*> TexturesToAcquire;

	WEngine::WArray<class RHITexture*> TexturesToDiscard;

	EPassFlag Flag;

	uint8 bHasUAVResource : 1;

	uint8 bSkipRenderPass : 1;

	uint8 bSkipRenderPassBegin : 1;

	uint8 bSkipRenderPassEnd : 1;

	WRDGPassHandle PrologueRenderPass;

	WRDGPassHandle EpilogueRenderPass;

	friend WRDGPassRegistry;

	friend class WRDGBuilder;

};

template<typename ParameterStructType, typename LAMBDA>
class WRDGLambdaPass : public WRDGPass
{
public:

	WRDGLambdaPass(WEngine::WString&& inName, const ParameterStructType* inParameters, LAMBDA inLambda)
		: WRDGPass(inName, WRDGParameterStruct(inParameters)),
		  Lambda(inLambda)
	{
	}

	virtual ~WRDGLambdaPass();

	virtual void Execute() override;

private:

	LAMBDA Lambda;

};

class WRDGBuilder
{
public:

	WRDGBuilder();

	~WRDGBuilder();

	void Compile();

	void Execute();

	template<typename ParameterStructType>
	ParameterStructType* AllocateParameterStruct();

	class WRDGTexture* CreateTexture(const WRDGTextureDesc& inDesc, const char* inName);

	class WRDGBuffer* CreateBuffer(const WRDGBufferDesc& inDesc, const char* inName);

	template<typename ParameterStructType, typename LAMBDA>
	WRDGPass* AddPass(WEngine::WString inName, const ParameterStructType* inParameters, LAMBDA inLambda);

private:

	void PassCulling();

	void PassMerging();

	void SetupPass(WRDGPass *Pass);

private:

	WRDGPassRegistry Passes;

	WRDGTextureRegistry Textures;

	WRDGBufferRegistry Buffers;

	WRDGPassBitArray PassesToCull;

	WRDGPassBitArray PassesWithEmptyParameters;

};

template<typename ParameterStructType>
inline ParameterStructType* WRDGBuilder::AllocateParameterStruct()
{
	return (ParameterStructType*)WRDGAllocator::Allocate(sizeof(ParameterStructType));
}

template<typename ParameterStructType, typename LAMBDA>
inline WRDGPass* WRDGBuilder::AddPass(WEngine::WString inName, const ParameterStructType* inParameters, LAMBDA inLambda)
{
	WRDGLambdaPass<ParameterStructType, LAMBDA> *Pass = Passes.Allocate<WRDGLambdaPass<ParameterStructType, LAMBDA>>(inName, inParameters, inLambda);
	SetupPass(Pass);
	return Pass;
}