#pragma once

#include "engine/misc/cTimer.h"
#include "engine/misc/iSingleton.h"

class cApplication final : public df::iSingleton< cApplication >
{
public:
	DF_DISABLE_COPY_AND_MOVE( cApplication );

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
