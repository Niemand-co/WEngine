#pragma once
#include "HAL/Public/WRunnable.h"
#include "HAL/Public/WEvent.h"
#include "HAL/Public/PlatformProcess.h"
#include "HAL/Public/Platform.h"

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
		None               = 0u,
		GameThread         = 1u,
		RenderThread       = 1u << 1,
		RHIThread          = 1u << 2,
						   
		NamedThreadNum     = 3u,
						   
		AnyThread          = 1u << 3,
						   
		ThreadIndexMask    = 15u,
						   
		HighPriority       = 1u << 4,
		NormalPriority     = 1u << 5,
		LowPriority        = 1u << 6,

		ThreadPriorityMask = 112u,
	};

	EThreadProperty GetThreadName(EThreadProperty property);

	EThreadProperty GetThreadPriority(EThreadProperty property);

	unsigned int GetQueueIndex(EThreadProperty property);

	class WTaskGraph
	{
	public:

		WTaskGraph();

		~WTaskGraph();

		void AttachToThread(EThreadProperty property);

		void ProcessUntilQuit(EThreadProperty property);

		void ProcessUntilIdle(EThreadProperty property);

		void EnqueTask(class WGraphTaskBase* task, EThreadProperty property);

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	public:

		static WTaskGraph* Get() { if(g_instance == nullptr)g_instance = new WTaskGraph(); return g_instance; }

	private:

		static WTaskGraph *g_instance;

	private:

		WThreadPack* m_threads[NUM_WORKING_THREAD];

	};

	class WGraphEvent
	{
	public:

		WGraphEvent()
			: m_event(PlatformProcess::CreateWEvent())
		{
		}

		~WGraphEvent()
		{
		}

		void Wait()
		{
			m_event->Wait();
		}

		void Trigger()
		{
			m_event->Trigger();
		}

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	private:

		WSharedPtr<WEvent> m_event;

		WQueue<class WGraphTaskBase*> m_waitingTasks;

	};

	class WGraphTaskBase
	{
	public:

		virtual bool ExecuteTask(EThreadProperty property) = 0;

		template<typename TTask, typename... Args>
		static WGraphTaskBase* CreateTask(Args... args)
		{
			return new TTask(args...);
		}

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

		WGraphTaskBase(bool bInShouldDestroy)
			: m_pTaskEvent(new WGraphEvent()), m_bShouldDestroyAfterExecution(bInShouldDestroy)
		{

		}

		~WGraphTaskBase() = default;

	protected:

		WSharedPtr<WGraphEvent> m_pTaskEvent;

		uint8 m_bShouldDestroyAfterExecution : 1;

	};

	class WTriggerTask : public WGraphTaskBase
	{
	public:

		WTriggerTask(bool bInShouldDestroy)
			: WGraphTaskBase(bInShouldDestroy)
		{

		}

		virtual ~WTriggerTask() = default;

		virtual bool ExecuteTask(EThreadProperty property) override
		{
			m_pTaskEvent->Trigger();
			return m_bShouldDestroyAfterExecution;
		}

		void Wait()
		{
			m_pTaskEvent->Wait();
		}

	};

	template<typename Lambda>
	class WLambdaTask : public WGraphTaskBase
	{
	public:

		WLambdaTask(bool bInShouldDestroy, Lambda inLambda)
			: WGraphTaskBase(bInShouldDestroy), lambda(inLambda)
		{
		}

		virtual ~WLambdaTask() = default;

		virtual bool ExecuteTask(EThreadProperty property) override
		{
			lambda();
			m_pTaskEvent->Trigger();
			return m_bShouldDestroyAfterExecution;
		}

	private:

		Lambda lambda;

	};

	class WTaskThreadBase
	{
	public:

		friend class WTaskGraph;

		WTaskThreadBase() = default;

		virtual ~WTaskThreadBase();

		virtual void ProcessUntilQuit() = 0;

		virtual void ProcessUntilIdle() = 0;

		virtual void RequistForQuit() = 0;

	protected:

		EThreadProperty m_threadId = EThreadProperty::None;

		struct WGraphTaskQueue
		{
			WQueue<WGraphTaskBase*> TaskQueue;

			class WEvent *StallingEvent = nullptr;

			uint8_t bQuitForReturn : 1 = false;

			uint8_t bQuitForIdle : 1 = false;
		};

		WGraphTaskQueue m_taskQueue;

	};

	class WNamedTaskThread : public WTaskThreadBase
	{
	public:

		WNamedTaskThread();

		virtual ~WNamedTaskThread();

		virtual void ProcessUntilQuit() override;

		virtual void ProcessUntilIdle() override;

		virtual void RequistForQuit() override;

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