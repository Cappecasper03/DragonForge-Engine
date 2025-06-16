#pragma once

#include "engine/core/utils/cTimer.h"
#include "engine/core/utils/iSingleton.h"

namespace df
{
	class cApplication final : public iSingleton< cApplication >
	{
	public:
		DF_DeleteCopyAndMove( cApplication );

		cApplication();
		~cApplication() override;

		static void run();
		static void quit();

		static const std::string& getName() { return getInstance()->m_name; }

	private:
		void initializeEngine();

		cTimer m_timer;

		bool        m_running;
		std::string m_name;
	};
}
