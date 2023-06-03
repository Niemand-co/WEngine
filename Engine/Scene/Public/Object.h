#pragma once
#include "Event/Public/TimeStep.h"

namespace WEngine
{
	class Editor;
}

class Object
{
	friend class GWorld;

	friend class WEngine::Editor;

public:

	virtual ~Object() = default;

	void* operator new(size_t size)
	{
		return GetCPUAllocator()->Allocate(size);
	}

	void operator delete(void *ptr)
	{
		GetCPUAllocator()->Deallocate(ptr);
	}

};

template<typename T>
T* GetCopy(T* object)
{
	T *copy = (T*)GetCPUAllocator()->Allocate(sizeof(T));
	memcpy(copy, object, sizeof(T));
	return copy;
}