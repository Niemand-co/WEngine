#include "pch.h"
#include "Utils/Allocator/Public/Allocator.h"

Block* Allocator::cur = nullptr;

Block::Block()
{
	data = (BYTE*)malloc(4096);
	cur = data;
	left = 4096;
	next = nullptr;
}

Block::~Block()
{
	cur = nullptr;
	delete data;
}

Allocator::Allocator()
{
	head = new Block();
	cur = head;
}

Allocator::~Allocator()
{
	while (head != nullptr)
	{
		Block* tmp = head;
		head = head->next;
		delete tmp;
	}
}

void* Allocator::Allocate(size_t size)
{
	if (size > cur->left)
	{
		cur->next = new Block();
	}

	BYTE* p = cur->cur;
	cur->cur += size;
	cur->left -= size;
	return p;
}