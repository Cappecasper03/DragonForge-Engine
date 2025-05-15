#pragma once

#include "engine/core/utils/cTimer.h"
#include "engine/core/utils/iSingleton.h"

class cApplication final : public df::iSingleton< cApplication >
{
public:
	DF_DeleteCopyAndMove( cApplication );

	cApplication();
	~cApplication() override;

	static void run();
	static void quit();

	static int getFps() { return static_cast< int >( getInstance()->m_fps ); }

	static double getDeltaTime() { return getInstance()->m_timer.getDeltaSecond(); }
	static double getLifeTime() { return getInstance()->m_timer.getLifeSecond(); }

	static const std::string& getName() { return getInstance()->m_name; }

private:
	void initializeEngine();

	df::cTimer m_timer;
	double     m_fps;

	bool        m_running;
	std::string m_name;
};
