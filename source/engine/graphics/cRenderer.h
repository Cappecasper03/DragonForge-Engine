#pragma once

#include "engine/core/utils/iSingleton.h"

namespace df
{
	class iRenderer;

	class cRenderer final : public iSingleton< cRenderer >
	{
	public:
		DF_DeleteCopyAndMove( cRenderer );

		enum eInstanceType
		{
			kOpenGl,
			kVulkan,
		};

		explicit cRenderer( eInstanceType _type, const std::string& _window_name );
		~cRenderer() override;

		static iRenderer*    getRenderInstance() { return getInstance()->m_instance; }
		static eInstanceType getInstanceType() { return getInstance()->m_type; }
		static bool          isDeferred() { return getInstance()->m_is_deferred; }

	private:
		iRenderer*    m_instance;
		eInstanceType m_type;
		bool          m_is_deferred;
	};
}
