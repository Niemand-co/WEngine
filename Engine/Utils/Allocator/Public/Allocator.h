#pragma once

class Allocator;

struct Block
{
	friend class Allocator;

public:

	Block();

	~Block();

private:

	BYTE *data;

	BYTE *cur;

	size_t left;

	Block *next;

};

class Allocator
{
public:

	Allocator();

	Allocator(const Allocator&) = delete;

	~Allocator();

	static void* Allocate(size_t size);

private:

	Block* head;

	static Block* cur;

};