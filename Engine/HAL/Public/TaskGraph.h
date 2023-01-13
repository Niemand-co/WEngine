#pragma once
#include "HAL/Public/WRunnable.h"

#define NUM_WORKING_THREAD 4

namespace WEngine
{

	class WTaskThreadBase;
	class WThread;

	struct WThreadPack
	{
		WTaskThreadBase *pTaskThread = nullptr;

		WThread         *pThread     = nullptr;

		uint8_t          bAttached   = false;
	};

	enum EThreadProperty : unsigned int
	{
		GameThread         = 0u,
		RenderThread       = 1u,
		RHIThread          = 1u << 1,
						   
		NamedThreadNum     = 3u,
						   
		AnyThread          = 1u << 2,
						   
		ThreadIndexMask    = 7u,
						   
		HighPriority       = 1u << 3,
		NormalPriority     = 1u << 4,
		LowPriority        = 1u << 5,

		ThreadPriorityMask = 56u,
	};

	EThreadProperty GetThreadName(EThreadProperty property)
	{
		return EThreadProperty(EThreadProperty::NamedThreadNum & property);
	}

	EThreadProperty GetThreadPriority(EThreadProperty property)
	{
		return EThreadProperty(EThreadProperty::ThreadPriorityMask & property);
	}

	class WTaskGraph
	{
	public:

		WTaskGraph();

		~WTaskGraph();

		template<typename LAMBDA>
		void CreateTask(LAMBDA lambda);

		void AttachToThread(EThreadProperty property);

	public:

		static WTaskGraph* Get() { return g_instance; }

	private:

		static WTaskGraph *g_instance;

	private:

		WThreadPack* m_threads[NUM_WORKING_THREAD];

	};

	class WGraphTask
	{
	public:

		WGraphTask();

		~WGraphTask();

	};

	class WTaskThreadBase
	{
	public:

		WTaskThreadBase();

		virtual ~WTaskThreadBase();

		virtual void ProcessUntilQuit() = 0;

		virtual void ProcessUntilIdle() = 0;

	private:

		EThreadProperty m_threadId;

		

	};

	class WNamedTaskThread : public WTaskThreadBase
	{
	public:

		WNamedTaskThread();

		virtual ~WNamedTaskThread();

		virtual void ProcessUntilQuit() override;

		virtual void ProcessUntilIdle() override;

		void ProcessNamedTaskThread();

	};

	class WAnyTaskThread : public WTaskThreadBase
	{
	public:

		WAnyTaskThread();

		virtual ~WAnyTaskThread();

		virtual void ProcessUntilQuit() override;

		virtual void ProcessUntilIdle() override;



	};

}