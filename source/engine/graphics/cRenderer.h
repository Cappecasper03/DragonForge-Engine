#pragma once

#include "engine/core/utils/cTimer.h"
#include "engine/core/utils/iSingleton.h"

namespace df
{
	class iGraphicsApi;

	class cRenderer final : public iSingleton< cRenderer >
	{
	public:
		DF_DeleteCopyAndMove( cRenderer );

		enum eGraphicsApi
		{
			kOpenGl,
			kVulkan,
		};

		explicit cRenderer( eGraphicsApi _type, const std::string& _window_name );
		~cRenderer() override;

		static iGraphicsApi* getApi() { return getInstance()->m_graphics_api; }
		static eGraphicsApi  getApiType() { return getInstance()->m_type; }
		static bool          isDeferred() { return getInstance()->m_is_deferred; }

		static double getLifeTime() { return getInstance()->m_timer.getLifeSecond(); }

	private:
		iGraphicsApi* m_graphics_api;
		eGraphicsApi  m_type;
		bool          m_is_deferred;

		cTimer m_timer;
	};
}
