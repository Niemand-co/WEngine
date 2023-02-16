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
	PassCulling();

	for (WRDGPassHandle Handle = Passes.Begin(); Handle != Passes.End(); ++Handle)
	{
		if (PassesToCull(Handle))
		{
			continue;
		}

		WRDGPass* Pass = Passes[Handle];
		bool bWithAsyncCompute = WEngine::EnumHasFlags(Pass->Flag, EPassFlag::AsyncCompute);

		for (const auto& TexturePair : Pass->TextureStates)
		{
			WRDGTexture* Texture = TexturePair.First();
			auto PassNeedState = TexturePair.Second();
			WEngine::WArray<WRDGResourceState*>& TextureState = Texture->GetMergeState();

			uint32 SubresourceCount = PassNeedState.States.Size();
			PassNeedState.MergeStates.Resize(SubresourceCount);
			if (TextureState.Size() != SubresourceCount)
			{
				InitResource(TextureState, SubresourceCount);
			}

			for (uint32 Index = 0; Index < SubresourceCount; ++Index)
			{
				if (!TextureState[Index] || !WRDGResourceState::IsMergeAllowed(*TextureState[Index], PassNeedState.States[Index]))
				{
					if (TextureState[Index] && TextureState[Index]->Pipeline != PassNeedState.States[Index].Pipeline)
					{

					}

					memcpy(&TextureState[Index], &PassNeedState.States[Index], sizeof(WRDGResourceState));
					TextureState[Index]->FirstPass = TextureState[Index]->LastPass = Handle;
				}
				else
				{
					TextureState[Index]->Access |= PassNeedState.States[Index].Access;
					TextureState[Index]->LastPass = Handle;
				}
			}
		}
	}
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
		for (const auto& TexturePair : Pass->TextureStates)
		{
			bUntrackedOutput |= TexturePair.First()->IsExternal();
		}
		for (const auto& BufferPair : Pass->BufferStates)
		{
			bUntrackedOutput |= BufferPair.First()->IsExternal();
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

}

void WRDGBuilder::SetupPass(WRDGPass* Pass)
{
	
}
