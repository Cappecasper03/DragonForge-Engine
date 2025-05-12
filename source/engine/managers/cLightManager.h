#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "engine/core/utils/iSingleton.h"
#include "engine/graphics/lights/sLight.h"

namespace df
{
	class cLightManager final : public iSingleton< cLightManager >
	{
	public:
		DF_DisableCopyAndMove( cLightManager );

		cLightManager()           = default;
		~cLightManager() override = default;

		static sLight& create( const std::string& _name, const sLight& _light );

		static bool destroy( const std::string& _name );
		static void clear();

		static const sLight& get( const std::string& _name );

		static const std::vector< sLight >& getLights() { return getInstance()->m_lights; }

		static constexpr unsigned m_max_lights = 512;

	private:
		std::unordered_map< std::string, unsigned > m_name_index;
		std::unordered_map< unsigned, std::string > m_index_name;
		std::vector< sLight >                       m_lights;
	};
}
