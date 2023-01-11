#pragma once
#include "Event/Public/TimeStep.h"

namespace WEngine
{
	class Editor;
}

class Object
{
	friend class World;

	friend class WEngine::Editor;

public:

	virtual ~Object() = default;

protected:

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void *ptr)
	{
		WEngine::Allocator::Get()->Deallocate(ptr);
	}

};

template<typename T>
T* GetCopy(T* object)
{
	T *copy = WEngine::Allocator::Get()->Allocate(sizeof(T));
	memcpy(copy, object, sizeof(T));
	return copy;
}