#pragma once

#include <string>

#include "engine/core/utils/iSingleton.h"
#include "engine/graphics/lights/sLight.h"

#define DF_MaxLights 64

namespace df
{
	class cLightManager final : public iSingleton< cLightManager >
	{
	public:
		DF_DisableCopyAndMove( cLightManager );

		struct sLightUniform
		{
			sLight   lights[ DF_MaxLights ];
			unsigned light_count = 0;
		};

		cLightManager()           = default;
		~cLightManager() override = default;

		static sLight& create( const std::string& _name, const sLight& _light );

		static bool destroy( const std::string& _name );
		static void clear();

		static const sLight& get( const std::string& _name );

		static const sLightUniform& getUniform() { return getInstance()->m_uniform; }

	private:
		std::string   m_light_names[ DF_MaxLights ];
		sLightUniform m_uniform;
	};
}
