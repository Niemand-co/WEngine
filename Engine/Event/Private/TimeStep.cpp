#include "pch.h"
#include "Event/Public/TimeStep.h"

namespace WEngine
{

	double TimeStep::g_lastFrameTime = 0.0l;

	TimeStep::TimeStep(float deltaTime)
		: m_deltaTime(deltaTime)
	{

	}

	double TimeStep::GetSeconds()
	{
		return m_deltaTime;
	}

	double TimeStep::GetMillSeconds()
	{
		return m_deltaTime * 1000.0l;
	}

	TimeStep TimeStep::GetTimeStep()
	{
		double time = glfwGetTime();
		TimeStep timeStep = time - g_lastFrameTime;
		g_lastFrameTime = time;
		return timeStep;
	}

}