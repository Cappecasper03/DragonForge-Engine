#pragma once

#include "engine/misc/iSingleton.h"

namespace df
{
	class iRenderer;

	class cRendererSingleton final : public iSingleton< cRendererSingleton >
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cRendererSingleton );

		enum eInstanceType
		{
			kOpenGL = 1 << 0,
			kVulkan = 1 << 1,
		};

		explicit cRendererSingleton( eInstanceType _type );
		~cRendererSingleton() override;

		static iRenderer*    getRenderInstance() { return getInstance()->m_instance; }
		static eInstanceType getRenderInstanceType() { return getInstance()->m_type; }

	private:
		iRenderer*    m_instance;
		eInstanceType m_type;
	};
};
