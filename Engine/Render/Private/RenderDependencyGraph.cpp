#include "pch.h"
#include "Render/Public/RenderDependencyGraph.h"
#include "Render/Public/RenderDependencyGraphResource.h"

WEngine::Allocator<6>* WEngine::Allocator<6>::g_pInstance = new WRDGAllocator();

WRDGBuilder::WRDGBuilder()
{
}

WRDGBuilder::~WRDGBuilder()
{
}

void WRDGBuilder::Compile()
{
}

void WRDGBuilder::Execute()
{
}

WRDGTexture* WRDGBuilder::CreateTexture(const WRDGTextureDesc& inDesc, const char* inName)
{
	WRDGTexture *Texture = Textures.Allocate(inDesc, inName);
	return Texture;
}

WRDGBuffer* WRDGBuilder::CreateBuffer(const WRDGBufferDesc& inDesc, const char* inName)
{
	WRDGBuffer *Buffer = Buffers.Allocate(inDesc, inName);
	return Buffer;
}

void WRDGBuilder::PassCulling()
{
	WEngine::WStack<WRDGPass*> PassesProcessing;
	WEngine::WStack<WRDGPass*> PassesWaiting;

	for (WRDGPassHandle Handle = Passes.Begin(); Handle < Passes.End(); Handle++)
	{
		for (WRDGTexture* Texture : Passes[Handle]->Textures)
		{
			if (Texture->IsExternal())
			{
				PassesProcessing.Push(Passes[Handle]);
				break;
			}
		}
		Passes[Handle]->MarkCulling(true);
		PassesWaiting.Push(Passes[Handle]);
	}

	while (!PassesProcessing.Empty())
	{

	}
}

void WRDGBuilder::PassMerging()
{
}
