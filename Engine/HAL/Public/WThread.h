#pragma once

namespace WEngine
{

	class WRunnable;

	class WThread
	{
	public:

		enum class ThreadPriority
		{
			Normal = 0,
			Lowest,
			Highest,
		};

		virtual ~WThread() = default;

		virtual void Setup() = 0;

		virtual void CreateInternal() = 0;

		unsigned int Run();

		size_t GetPID() const { return m_id; }

	protected:

		WString m_name;

		unsigned long m_id;

		size_t m_threadStackSize;

		ThreadPriority m_priority;

		WRunnable *m_pRunnable;

	};

}