#pragma once

namespace WEngine
{

	class EngineBase
	{
	public:

		virtual void PreInit() = 0;

		virtual void Init() = 0;

		virtual void Tick(const class TimeStep& timeStep) = 0;

		void* operator new(size_t size)
		{
			return GetCPUAllocator()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			GetCPUAllocator()->Deallocate(pData);
		}

	protected:

		EngineBase() = default;

		virtual ~EngineBase() = default;

	};

	class GEngine final : public EngineBase
	{
	public:

		GEngine();

		virtual ~GEngine();

		virtual void PreInit() override;

		virtual void Init() override;

		virtual void Tick(const TimeStep& timeStep) override;

	public:

		static GEngine* Get() { if(g_instance == nullptr)g_instance = new GEngine(); return g_instance; }

	private:

		static GEngine* g_instance;

	};

	class REngine final : public EngineBase
	{
	public:

		REngine();

		virtual ~REngine();

		virtual void PreInit() override;

		virtual void Init() override;

		virtual void Tick(const TimeStep& timeStep) override;

		class RHIContext* GetContext() const { return m_pContext; }

	public:

		static REngine* Get() { if(g_instance == nullptr)g_instance = new REngine(); return g_instance; }

	private:

		void InitRHIDevice();

		void ResourceLoading();

		void StartRenderingThread();

	private:

		static REngine *g_instance;

	private:

		RHIContext *m_pContext;

		class WRenderingThread *m_pRenderingRunnable;

		class WThread *m_pRenderingThread;

	};

	class FrameSync
	{
	public:

		static void Sync();

	private:

		static class WTriggerTask SyncEvent[2];

		static uint32 FrameIndex;

	};

}