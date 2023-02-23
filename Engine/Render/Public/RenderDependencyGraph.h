#pragma once
#include "Render/Public/RenderDependencyGraphParameter.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"

class WRDGBarrierBatch : public RHIBarrierBatch
{
public:

	WRDGBarrierBatch() = default;

	virtual ~WRDGBarrierBatch() = default;

	void Submit(RHIRenderCommandList& CmdList);

private:

	WEngine::WArray<class RHIBarrierDescriptor> BarrierDesc;

};

class WRDGPass
{
public:

	WRDGPass(WEngine::WString&& inName,  WRDGParameterStruct inParameters)
		: Name(inName),
		  Parameters(inParameters)
	{
	}

	virtual ~WRDGPass() = default;

	virtual void Execute() = 0;

	void SetNeverCulling(bool bInCulling) { Flag = (EPassFlag)((uint16)Flag | (uint16)EPassFlag::NeverCull); }

	bool IsNeverCulling() const { return ((uint16)Flag & (uint16)EPassFlag::NeverCull) != 0; }

	EPipeline GetPipeline() const { if(Flag == EPassFlag::AsyncCompute) return EPipeline::AsyncCompute; else return EPipeline::Graphics; }

private:

	struct TextureState
	{
		WEngine::WArray<WRDGResourceState> States;
		WEngine::WArray<WRDGResourceState*> MergeStates;
		uint32 ReferenceCount;
	};

	struct BufferState
	{
		WRDGResourceState State;
		WRDGResourceState* MergeState;
		uint32 ReferenceCount;
	};

	WRDGPassHandle Handle;

	WEngine::WString Name;

	const WRDGParameterStruct Parameters;

	WRDGPassHandleArray Producers;

	WEngine::WHashMap<class WRDGBuffer*, BufferState> BufferStates;

	WEngine::WHashMap<class WRDGTexture*, TextureState> TextureStates;

	WEngine::WArray<class RHITexture*> TexturesToAcquire;

	WEngine::WArray<class RHITexture*> TexturesToDiscard;

	EPassFlag Flag;

	uint8 bHasUAVResource : 1;

	uint8 bSkipRenderPass : 1;

	uint8 bSkipRenderPassBegin : 1;

	uint8 bSkipRenderPassEnd : 1;

	WRDGPassHandle PrologueRenderPass;

	WRDGPassHandle EpilogueRenderPass;

	friend WRDGPassRegistry;

	friend class WRDGBuilder;

};

template<typename ParameterStructType, typename LAMBDA>
class WRDGLambdaPass : public WRDGPass
{
public:

	WRDGLambdaPass(WEngine::WString&& inName, const ParameterStructType* inParameters, LAMBDA inLambda)
		: WRDGPass(inName, WRDGParameterStruct(inParameters)),
		  Lambda(inLambda)
	{
	}

	virtual ~WRDGLambdaPass();

	virtual void Execute() override;

private:

	LAMBDA Lambda;

};

class WRDGBuilder
{
public:

	WRDGBuilder();

	~WRDGBuilder();

	void Compile();

	void Execute();

	template<typename ParameterStructType>
	ParameterStructType* AllocateParameterStruct();

	class WRDGTexture* CreateTexture(const WRDGTextureDesc& inDesc, const char* inName);

	class WRDGBuffer* CreateBuffer(const WRDGBufferDesc& inDesc, const char* inName);

	template<typename ParameterStructType, typename LAMBDA>
	WRDGPass* AddPass(WEngine::WString inName, const ParameterStructType* inParameters, LAMBDA inLambda);

private:

	void PassCulling();

	void PassMerging();

	void SetupPass(WRDGPass *Pass);

	void BeginResourceRHI(EUniformBaseType Type, WRDGPassHandle PassHandle, WRDGTexture* Texture);

	void BeginResourceRHI(EUniformBaseType Type, WRDGPassHandle PassHandle, WRDGBuffer* Buffer);

	void BeginResourceRHI(EUniformBaseType Type, WRDGPassHandle PassHandle, class WRDGTextureSRV* SRV);

	void BeginResourceRHI(EUniformBaseType Type, WRDGPassHandle PassHandle, class WRDGTextureUAV* UAV);

	void EndResourceRHI(WRDGPassHandle PassHandle, class WRDGTexture* Texture);

	void EndResourceRHI(WRDGPassHandle PassHandle, class WRDGTextureSRV* Texture);

	void EndResourceRHI(WRDGPassHandle PassHandle, class WRDGTextureUAV* Texture);

	void EndResourceRHI(WRDGPassHandle PassHandle, class WRDGBuffer* Texture);

private:

	WRDGPassRegistry Passes;

	WRDGTextureRegistry Textures;

	WRDGBufferRegistry Buffers;

	WRDGPassBitArray PassesToCull;

	WRDGPassBitArray PassesWithEmptyParameters;

};

template<typename ParameterStructType>
inline ParameterStructType* WRDGBuilder::AllocateParameterStruct()
{
	return (ParameterStructType*)WRDGAllocator::Allocate(sizeof(ParameterStructType));
}

template<typename ParameterStructType, typename LAMBDA>
inline WRDGPass* WRDGBuilder::AddPass(WEngine::WString inName, const ParameterStructType* inParameters, LAMBDA inLambda)
{
	WRDGLambdaPass<ParameterStructType, LAMBDA> *Pass = Passes.Allocate<WRDGLambdaPass<ParameterStructType, LAMBDA>>(inName, inParameters, inLambda);
	SetupPass(Pass);
	return Pass;
}