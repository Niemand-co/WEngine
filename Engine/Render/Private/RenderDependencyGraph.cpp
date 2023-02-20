#include "pch.h"
#include "Render/Public/RenderDependencyGraph.h"
#include "Render/Public/RenderDependencyGraphResource.h"
#include "Render/Public/RenderDependencyGraphParameter.h"

WEngine::Allocator<6>* WEngine::Allocator<6>::g_pInstance = new WRDGAllocator();

inline EAccess GetValidWriteAccess(const EAccess& Access)
{
	return ((uint16)(Access & EAccess::Writable) != 0) ? (Access & ~EAccess::ReadOnly) : Access;
}

inline EAccess GetValidReadAccess(const EAccess& Access)
{
	return ((uint16)(Access & EAccess::Readable) != 0) ? (Access & ~EAccess::WriteOnly) : Access;
}



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

		Pass->TextureStates.Enumerate([this, &Handle](WEngine::WPair<WRDGTexture*, WRDGPass::TextureState>& TexturePair)
		{
			WRDGTexture* Texture = TexturePair.First();
			auto& PassNeedState = TexturePair.Second();
			Texture->ReferenceCount+= PassNeedState.ReferenceCount;

			WEngine::WArray<WRDGResourceState*>& TextureState = Texture->GetMergeState();

			uint32 SubresourceCount = PassNeedState.States.Size();
			PassNeedState.MergeStates.Resize(SubresourceCount);
			if (TextureState.Size() != SubresourceCount)
			{
				InitResource(TextureState, SubresourceCount);
			}

			for (uint32 Index = 0; Index < SubresourceCount; ++Index)
			{
				if (PassNeedState.States[Index].Access == EAccess::Unknown)
				{
					continue;
				}

				if (!TextureState[Index] || !WRDGResourceState::IsMergeAllowed(*TextureState[Index], PassNeedState.States[Index]))
				{
					if (TextureState[Index] && TextureState[Index]->Pipeline != PassNeedState.States[Index].Pipeline)
					{
						if (Passes[Handle]->Producers.Find(TextureState[Index]->LastPass) == Passes[Handle]->Producers.end())
						{
							Passes[Handle]->Producers.Push(TextureState[Index]->LastPass);
						}
					}

					TextureState[Index] = (WRDGResourceState*)(WRDGAllocator::Get()->Allocate(sizeof(WRDGResourceState)));
					memcpy(TextureState[Index], &PassNeedState.States[Index], sizeof(WRDGResourceState));
					TextureState[Index]->FirstPass = TextureState[Index]->LastPass = Handle;
				}
				else
				{
					TextureState[Index]->Access |= PassNeedState.States[Index].Access;
					TextureState[Index]->LastPass = Handle;
				}
				PassNeedState.MergeStates = TextureState;
			}
		});

		Pass->BufferStates.Enumerate([this, &Handle](WEngine::WPair<WRDGBuffer*, WRDGPass::BufferState>& BufferPair)
		{
			WRDGBuffer* Buffer = BufferPair.First();
			auto& PassNeedState = BufferPair.Second();
			Buffer->ReferenceCount += PassNeedState.ReferenceCount;

			if (PassNeedState.State.Access == EAccess::Unknown)
			{
				return;
			}

			if (!Buffer->MergeState || !WRDGResourceState::IsMergeAllowed(*Buffer->MergeState, PassNeedState.State))
			{
				if (Buffer->MergeState && PassNeedState.State.Pipeline != Buffer->MergeState->Pipeline)
				{
					if (Passes[Handle]->Producers.Find(Buffer->MergeState->LastPass) == Passes[Handle]->Producers.end())
					{
						Passes[Handle]->Producers.Push(Buffer->MergeState->LastPass);
					}

					Buffer->MergeState = (WRDGResourceState*)(WRDGAllocator::Get()->Allocate(sizeof(WRDGResourceState)));
					memcpy(Buffer->MergeState, &PassNeedState.State, sizeof(WRDGResourceState));
					Buffer->MergeState->FirstPass = Buffer->MergeState->LastPass = Handle;
				}
				else
				{
					Buffer->MergeState->Access |= PassNeedState.State.Access;
					Buffer->MergeState->LastPass = Handle;
				}
				PassNeedState.MergeState = Buffer->MergeState;
			}
		});
	}
}

void WRDGBuilder::Execute()
{
}

WRDGTexture* WRDGBuilder::CreateTexture(const WRDGTextureDesc& inDesc, const char* inName)
{
	WRDGTexture *Texture = Textures.Allocate<WRDGTexture>(inDesc, inName);
	return Texture;
}

WRDGBuffer* WRDGBuilder::CreateBuffer(const WRDGBufferDesc& inDesc, const char* inName)
{
	WRDGBuffer *Buffer = Buffers.Allocate<WRDGBuffer>(inDesc, inName);
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
		bool bUntrackedOutput = Pass->Parameters.HasExternalOutput();
		Pass->TextureStates.Enumerate([&bUntrackedOutput](WEngine::WPair<WRDGTexture*, WRDGPass::TextureState>& TexturePair)
		{
			bUntrackedOutput |= TexturePair.First()->IsExternal();
		});

		Pass->BufferStates.Enumerate([&bUntrackedOutput](WEngine::WPair<WRDGTexture*, WRDGPass::BufferState>& BufferPair)
		{
			bUntrackedOutput |= BufferPair.First()->IsExternal();
		});

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
	WEngine::WArray<WRDGPassHandle> PassesToMerge;

	for (WRDGPassHandle Handle = Passes.Begin(); Handle != Passes.End(); ++Handle)
	{
		if (PassesToCull[Handle])
		{
			continue;
		}

		WRDGPassHandle PrePass = Handle;
		WRDGPassHandle NextPass = WRDGPassHandle(PrePass.Index + 1);
		PassesToMerge.Push(PrePass);
		while (NextPass != Passes.End() && Passes[NextPass]->Parameters.GetRenderTarget() == Passes[NextPass]->Parameters.GetRenderTarget() && Passes[NextPass]->bSkipRenderPass)
		{
			PassesToMerge.Push(NextPass);
			PrePass = NextPass;
			++NextPass;
		}

		if (PassesToMerge.Size() == 1)
		{
			WRDGPass *Pass = Passes[PassesToMerge[0]];
			Pass->bSkipRenderPassBegin = false;
			Pass->bSkipRenderPassEnd = false;
			Pass->PrologueRenderPass = PassesToMerge[0];
			Pass->EpilogueRenderPass = PassesToMerge[0];
		}

		WRDGPassHandle FirstPassHandle = PassesToMerge[0];
		WRDGPassHandle LastPassHandle = PassesToMerge[PassesToMerge.Size() - 1];

		{
			WRDGPass *FirstPass = Passes[FirstPassHandle];
			FirstPass->bSkipRenderPassEnd = true;
			FirstPass->bSkipRenderPassBegin = false;
			FirstPass->PrologueRenderPass = FirstPassHandle;
			FirstPass->EpilogueRenderPass = LastPassHandle;
		}

		{
			WRDGPass* LastPass = Passes[FirstPassHandle];
			LastPass->bSkipRenderPassEnd = false;
			LastPass->bSkipRenderPassBegin = true;
			LastPass->PrologueRenderPass = FirstPassHandle;
			LastPass->EpilogueRenderPass = LastPassHandle;
		}

	}
}

void WRDGBuilder::SetupPass(WRDGPass* Pass)
{
	const WRDGParameterStruct &Parameters = *Pass->Parameters;
	const WRDGPassHandle& Handle = Pass->Handle;
	EPipeline Pipeline = Pass->GetPipeline();

	Parameters.EnumerateTextures(Pass->Flag, [&Pass, &Pipeline, &Handle](EUniformBaseType Type, WRDGTexture* Texture, EAccess Access)
	{
		auto& PassState = Pass->TextureStates[Texture];
		PassState.ReferenceCount++;

		PassState.States.Resize(Texture->Layout.MipCount * Texture->Layout.LayerCount * Texture->Layout.PlaneCount);

		WRDGTextureSubresourceRange Range = Texture->GetSubresourceRange();
		Texture->EnumerateSubresource(Range, [&PassState, &Pipeline, &Handle, &Access](uint32 Index)
		{
			PassState.States[Index].Access = GetValidWriteAccess(PassState.States[Index].Access | Access);
			PassState.States[Index].Pipeline = Pipeline;
			PassState.States[Index].FirstPass = Handle;
			PassState.States[Index].LastPass = Handle;
		});

		Pass->bHasUAVResource |= WEngine::EnumHasFlags(Access, EAccess::UAV);
	});

	Parameters.EnumerateBuffers(Pass->Flag, [&Pass, &Pipeline, &Handle](EUniformBaseType Type, WRDGBuffer* Buffer, EAccess Access)
	{
		auto& BufferState = Pass->BufferStates[Buffer];
		BufferState.ReferenceCount++;

		BufferState.State.Access = GetValidWriteAccess(BufferState.State.Access | Access);
		BufferState.State.Pipeline = Pipeline;
		BufferState.State.FirstPass = Handle;
		BufferState.State.LastPass = Handle;

		Pass->bHasUAVResource |= WEngine::EnumHasFlags(Access, EAccess::UAV);
	});
}

void WRDGBarrierBatch::Submit(RHIRenderCommandList& CmdList)
{
}
