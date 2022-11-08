#pragma once

class ScriptableRenderPipeline;
class RHISemaphore;

namespace WEngine
{

	struct Trigger
	{
		bool IsNameIn(std::string_view name)
		{
			for(unsigned int i = 0; i < waitingSubmissionCount; ++i)
				if(*(pSubmissionNames + i) == name)
					return true;
			return false;
		}

		RHISemaphore *signal;
		unsigned int waitingSubmissionCount;
		std::string_view *pSubmissionNames;
	};

	class Synchronizer
	{
		enum class Backend
		{
			None = 0,
			Vulkan,
		};

	public:

		virtual ~Synchronizer() = default;

		virtual void SetEvent(unsigned int index) = 0;

		virtual void WaitEvent(unsigned int index) = 0;

	public:

		static void Init(Backend backend);

		static Synchronizer* Get();

		static void RegisterTrigger(Trigger *pTrigger);

		static std::vector<Trigger*> GetTrigger(std::string_view submissionName);

	private:

		static Synchronizer *g_pSynchronizer;

		static std::vector<Trigger*> g_pTriggers;

	};


}