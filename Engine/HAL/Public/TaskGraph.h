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

		template<typename LAMBDA>
		void CreateTask(LAMBDA lambda);

		void AttachToThread(EThreadProperty property);

		void ProcessUntilQuit(EThreadProperty property);

		void ProcessUntilIdle(EThreadProperty property);

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

	class WGraphTaskBase
	{
	public:

		virtual void operator()() = 0;

	protected:
		
		WGraphTaskBase() = default;

		~WGraphTaskBase() = default;

	};

	template<typename Task>
	class WGraphTask
	{
	public:

		WGraphTask(Task inTask)
			: task(inTask)
		{
		}

		template<typename... Args>
		void DispatchWhenReady(Args... args)
		{
			task(args...);
		}

	private:

		Task task;

	};

	class WTaskThreadBase
	{
	public:

		friend class WTaskGraph;

		WTaskThreadBase() = default;

		virtual ~WTaskThreadBase();

		virtual void ProcessUntilQuit() = 0;

		virtual void ProcessUntilIdle() = 0;

	protected:

		EThreadProperty m_threadId = EThreadProperty::None;

		struct WGraphTaskQueue
		{

		};

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