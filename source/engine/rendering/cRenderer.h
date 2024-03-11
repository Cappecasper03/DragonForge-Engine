#pragma once

#include "engine/misc/iSingleton.h"

namespace df
{
	class iRenderer;

	class cRenderer final : public iSingleton< cRenderer >
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cRenderer );

		enum eInstanceType
		{
			kOpenGL = 1 << 0,
			kVulkan = 1 << 1,
		};

		explicit cRenderer( eInstanceType _type );
		~cRenderer() override;

		static iRenderer*    getRenderInstance() { return getInstance()->m_instance; }
		static eInstanceType getInstanceType() { return getInstance()->m_type; }

	private:
		iRenderer*    m_instance;
		eInstanceType m_type;
	};
};
