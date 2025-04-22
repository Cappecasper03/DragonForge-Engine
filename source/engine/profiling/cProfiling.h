#pragma once

#ifdef DF_Profiling
	#include <thread>

namespace df
{
	class cProfiling
	{
	public:
		static void start();
		static void stop();

	private:
		static void profiling();

		static std::atomic< bool > m_running;
		static std::thread*        m_profiling_thread;
	};
}
#endif
