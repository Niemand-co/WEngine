#pragma once
#ifndef WENGINE_OBJECT_H
#define WENGINE_OBJECT_H

class Object
{
	friend class World;

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

#endif