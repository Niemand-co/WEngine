#include "pch.h"
#include "Render/Public/RenderDependencyGraph.h"
#include "Render/Public/RenderDependencyGraphResource.h"
#include "Render/Public/RenderDependencyGraphParameter.h"

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
	uint16 Count = Passes.End() - Passes.Begin();
	WRDGPassBitArray PassesNeverCull(Count, false);
	WRDGPassBitArray PassesWithUntrackedOutput(Count, true);
	PassesToCull.Init(Count, true);

	for (WRDGPassHandle Handle = Passes.Begin(); Handle != Passes.End(); ++Handle)
	{
		WRDGPass *Pass = Passes[Handle];
		bool bUntrackedOutput = false;
		for (const WRDGTexture* Texture : Pass->Textures)
		{
			bUntrackedOutput |= Texture->IsExternal();
		}
		for (const WRDGBuffer* Buffer : Pass->Buffers)
		{
			bUntrackedOutput |= Buffer->IsExternal();
		}

		PassesNeverCull[Handle] = Pass->IsNeverCulling();
		PassesWithUntrackedOutput[Handle] = bUntrackedOutput;
	}

	WEngine::WArray<WRDGPassHandle> PassesProcessing;

	for (WRDGPassHandle Handle = Passes.Begin(); Handle < Passes.End(); Handle++)
	{
		if(PassesNeverCull[Handle] || PassesWithUntrackedOutput[Handle])
			PassesProcessing.Push(Handle);
	}

	while (!PassesProcessing.Empty())
	{
		WRDGPassHandle PassHandle = PassesProcessing.Pop();
		if (PassesToCull[PassHandle])
		{
			PassesToCull[PassHandle] = false;
			PassesProcessing.Append(Passes[PassHandle]->Producers);
		}
	}
}

void WRDGBuilder::PassMerging()
{
	for (WRDGPassHandle Handle = Passes.Begin(); Handle != Passes.End(); ++Handle)
	{
		if (PassesToCull(Handle) || WEngine::EnumHasFlags(Passes[Handle]->Flag, EPassFlag::AsyncCompute))
		{
			continue;
		}

		WRDGPass *Pass = Passes[Handle];
		for(Pass->Textures)
	}
}
