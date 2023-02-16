#pragma once
#include "Render/Public/RenderDependencyGraphDefinitions.h"

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

	template<typename... Args>
	ObjectType* Allocate(Args... args)
	{
		ObjectType *object = (ObjectType*)WRDGAllocator::Get()->Allocate(sizeof(ObjectType));
		::new (object) Object(args...);
		Insert(object);
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

class WRDGPass
{
public:

	WRDGPass(WEngine::WString&& inName, const class WRDGParameterStruct* inParameters)
		: Name(inName),
		  Parameters(inParameters)
	{
	}

	virtual ~WRDGPass() = default;

	virtual void Execute() = 0;

	void SetNeverCulling(bool bInCulling) { Flag = (EPassFlag)((uint16)Flag | (uint16)EPassFlag::NeverCull); }

	bool IsNeverCulling() const { return ((uint16)Flag & (uint16)EPassFlag::NeverCull) != 0; }

private:

	struct TextureState
	{
		WEngine::WArray<WRDGResourceState> States;
		WEngine::WArray<WRDGResourceState> MergeStates;
		uint32 ReferenceCount;
	};

	struct BufferState
	{
		WEngine::WArray<WRDGResourceState> States;
		uint32 ReferenceCount;
	};

	WRDGPassHandle Handle;

	WEngine::WString Name;

	const WRDGParameterStruct* Parameters;

	WRDGPassHandleArray Producers;

	WEngine::WArray<WEngine::WPair<class WRDGBuffer*, BufferState>> BufferStates;

	WEngine::WArray<WEngine::WPair<class WRDGTexture*, TextureState>> TextureStates;

	WEngine::WArray<class RHITexture*> TexturesToAcquire;

	WEngine::WArray<class RHITexture*> TexturesToDiscard;

	EPassFlag Flag;

	friend WRDGPassRegistry;

	friend class WRDGBuilder;

};

template<typename LAMBDA>
class WRDGLambdaPass : public WRDGPass
{
public:

	WRDGLambdaPass(WEngine::WString&& inName, const WRDGParameters* inParameters, LAMBDA inLambda)
		: WRDGPass(inName, inParameters),
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
	WRDGLambdaPass *Pass = Passes.Allocate(inName, inParameters, inLambda);
	SetupPass(Pass);
	return Pass;
}