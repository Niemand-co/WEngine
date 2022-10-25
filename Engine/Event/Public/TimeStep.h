#pragma once

namespace WEngine
{

	class TimeStep
	{
	public:

		TimeStep(float deltaTime = 0.0f);

		~TimeStep() = default;

		double GetSeconds();

		double GetMillSeconds();

	public:

		static TimeStep GetTimeStep();

	private:

		double m_deltaTime;

	private:

		static double g_lastFrameTime;

	};

}