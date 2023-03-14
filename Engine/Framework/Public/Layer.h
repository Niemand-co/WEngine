#pragma once

class WViewport;
class RScene;
class CameraComponent;

namespace WEngine
{

	class Event;

	class Layer
	{
	public:

		virtual void OnAttach() = 0;

		virtual void OnDettach() = 0;

		virtual void OnEvent(Event *pEvent) = 0;

		virtual void Tick(TimeStep timeStep) = 0;

		virtual const WString& GetName() const
		{
			return DebugName;
		}

		void* operator new(size_t size)
		{
			return NormalAllocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			NormalAllocator::Get()->Deallocate(pData);
		}

	protected:

		Layer(const WString& name);

		virtual ~Layer() = default;

		void StartFrame();

		void EndFrame();

		void StartRendering();

	protected:

		WString DebugName;

		WEngine::WSharedPtr<WViewport> Viewport;

		WEngine::WSharedPtr<RScene> Scene;

		WEngine::WSharedPtr<CameraComponent> Camera;

	};

}