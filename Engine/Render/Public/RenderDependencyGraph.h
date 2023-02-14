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

struct WRDGRenderTargetBinding
{
	WRDGTexture *Texture;
	AttachmentLoadOP LoadOp;
};

struct WRDGParameters
{
	WEngine::WArray<WRDGRenderTargetBinding> RenderTargets;
};

class WRDGPass
{
public:

	WRDGPass(WEngine::WString&& inName, const WRDGParameters* inParameters)
		: Name(inName),
		  Parameters(inParameters)
	{
	}

	virtual ~WRDGPass() = default;

	virtual void Execute() = 0;

	void MarkCulling(bool inCulling) { bCulling = inCulling; }

	bool IsCulling() const { return bCulling; }

private:

	WRDGPassHandle Handle;

	WEngine::WString Name;

	const WRDGParameters* Parameters;

	WRDGPassHandleArray Producers;

	WEngine::WArray<class WRDGBuffer*> Buffers;

	WEngine::WArray<class WRDGTexture*> Textures;

	uint8 bCulling : 1;

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

	template<typename LAMBDA>
	void AddPass(WEngine::WString inName, const WRDGParameters* inParameters, LAMBDA inLambda);

	class WRDGTexture* CreateTexture(const WRDGTextureDesc& inDesc, const char* inName);

	class WRDGBuffer* CreateBuffer(const WRDGBufferDesc& inDesc, const char* inName);

private:

	void PassCulling();

	void PassMerging();

private:

	WRDGPassRegistry Passes;

	WRDGTextureRegistry Textures;

	WRDGBufferRegistry Buffers;

};

template<typename LAMBDA>
inline void WRDGBuilder::AddPass(WEngine::WString inName, const WRDGParameters* inParameters, LAMBDA inLambda)
{
}
