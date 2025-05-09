#pragma once

#include <string>

#include "engine/core/utils/iSingleton.h"
#include "engine/graphics/lights/sLight.h"

namespace df
{
	class cLightManager final : public iSingleton< cLightManager >
	{
	public:
		struct sLightUniform
		{
			sLight   lights[ 64 ];
			unsigned light_count = 0;
		};

		static sLight& create( const std::string& _name, const sLight& _light );

		static bool destroy( const std::string& _name );
		static void clear();

		static const sLight& get( const std::string& _name );

		static const sLightUniform& getUniform() { return getInstance()->m_uniform; }

	private:
		std::string   m_light_names[ 64 ];
		sLightUniform m_uniform;
	};
}
