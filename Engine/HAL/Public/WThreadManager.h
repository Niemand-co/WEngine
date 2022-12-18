#pragma once

namespace WEngine
{

	class WThread;

	class WThreadManager
	{
	public:

		WThreadManager();

		~WThreadManager();

	private:

		static WThreadManager *g_instance;

	private:

		WHashMap<size_t, WThread*> m_threads;

	};

}