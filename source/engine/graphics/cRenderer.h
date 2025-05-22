#pragma once

#include "engine/core/utils/cTimer.h"
#include "engine/core/utils/iSingleton.h"

namespace df
{
	class iGraphicsDevice;

	class cRenderer final : public iSingleton< cRenderer >
	{
	public:
		DF_DeleteCopyAndMove( cRenderer );

		enum eDeviceType
		{
			kOpenGl,
			kVulkan,
		};

		explicit cRenderer( eDeviceType _type, const std::string& _window_name );
		~cRenderer() override;

		static iGraphicsDevice* getGraphicsDevice() { return getInstance()->m_graphics_device; }
		static eDeviceType      getDeviceType() { return getInstance()->m_type; }
		static bool             isDeferred() { return getInstance()->m_is_deferred; }

		static double getLifeTime() { return getInstance()->m_timer.getLifeSecond(); }

	private:
		iGraphicsDevice* m_graphics_device;
		eDeviceType      m_type;
		bool             m_is_deferred;

		cTimer m_timer;
	};
}
