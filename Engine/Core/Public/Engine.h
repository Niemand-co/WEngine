#pragma once

namespace WEngine
{

	class EngineBase
	{
	public:

		virtual void PreInit() = 0;

		virtual void Init() = 0;

		virtual void Run() = 0;

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

		virtual void Run() override;

	};

	class REngine final : public EngineBase
	{
	public:

		REngine();

		virtual ~REngine();

		virtual void PreInit() override;

		virtual void Init() override;

		virtual void Run() override;

		class RHIContext* GetContext() const { return m_pContext; }

	public:

		static REngine* Get() { return g_instance; }

	private:

		void InitRHIDevice();

		void StartRenderingThread();

	private:

		static REngine *g_instance;

	private:

		RHIContext *m_pContext;

		class WRenderingThread *m_pRenderingRunnable;

		class WThread *m_pRenderingThread;

	};

}