#pragma once

class WRDGAllocator : public WEngine::Allocator<6>
{

};

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
	void operator==(IndexType inIndex) { RE_ASSERT(IsValid, "Handle Invalid."); return Index == inIndex; }
	void operator!=(IndexType inIndex) { RE_ASSERT(IsValid, "Handle Invalid."); return Index != inIndex; }
	void operator<=(IndexType inIndex) { RE_ASSERT(IsValid, "Handle Invalid."); return Index <= inIndex; }
	void operator>=(IndexType inIndex) { RE_ASSERT(IsValid, "Handle Invalid."); return Index >= inIndex; }
	void operator<(IndexType inIndex) { RE_ASSERT(IsValid, "Handle Invalid."); return Index < inIndex; }
	void operator>(IndexType inIndex) { RE_ASSERT(IsValid, "Handle Invalid."); return Index > inIndex; }

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

typedef WRDGHandle<class WRDGPass, uint16> WRDGPassHandle;

template<typename HandleType>
class WRDGHandleRegistry
{
public:

	typedef HandleType::ObjectType ObjectType

	WRDGHandleRegistry();

	~WRDGHandleRegistry();

	void Insert(ObjectType *Object)
	{
		Object->Handle = Array.Size();
		Array.Push(Object);
	}

	template<typename... Args>
	ObjectType* Allocator(Args... args)
	{
		ObjectType *object = (ObjectType*)WRDGAllocator::Get()->Allocate(sizeof(ObjectType));
		::new (object) Object(args...);
		Insert(object);
		return object;
	}

	Object* operator[](HandleType handle)
	{
		return Array[handle.Index];
	}

	const Object* operator[](HandleType handle) const
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

typedef WRDGHandleRegistry<class WRDGPass> WRDGPassRegistry;

class WRDGPass
{
public:

	WRDGPass();

	~WRDGPass();

private:

	WRDGPassHandle Handle;

	friend WRDGPassRegistry;

};

class WRDGBuilder
{
public:

	WRDGBuilder();

	~WRDGBuilder();

private:

	

	WEngine::WArray<

};