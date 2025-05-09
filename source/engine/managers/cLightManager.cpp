#include "cLightManager.h"

#include <fmt/format.h>

namespace df
{

	sLight& cLightManager::create( const std::string& _name, const sLight& _light )
	{
		DF_ProfilingScopeCpu;

		std::string*   names   = getInstance()->m_light_names;
		sLightUniform& uniform = getInstance()->m_uniform;

		for( int i = 0; i < 64; ++i )
		{
			if( names[ i ] == _name )
			{
				DF_LogWarning( fmt::format( "Light already exist: {}", _name ) );
				return uniform.lights[ 63 ];
			}
		}

		uniform.light_count++;
		names[ uniform.light_count - 1 ]          = _name;
		uniform.lights[ uniform.light_count - 1 ] = _light;
		DF_LogMessage( fmt::format( "Created light: {}", _name ) );
		return uniform.lights[ uniform.light_count - 1 ];
	}

	bool cLightManager::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::string*   names   = getInstance()->m_light_names;
		sLightUniform& uniform = getInstance()->m_uniform;
		bool           found   = false;

		for( unsigned i = 0; i < 64; ++i )
		{
			if( names[ i ] == _name && !found )
			{
				found = true;
				uniform.light_count--;
			}
			else if( found )
			{
				names[ i - 1 ]          = names[ i ];
				uniform.lights[ i - 1 ] = uniform.lights[ i ];
			}

			if( i > uniform.light_count )
				break;
		}

		if( found )
			DF_LogMessage( fmt::format( "Destroyed light: {}", _name ) );
		else
			DF_LogWarning( fmt::format( "Light doesn't exist: {}", _name ) );
		return found;
	}

	void cLightManager::clear()
	{
		DF_ProfilingScopeCpu;

		std::fill( std::begin( getInstance()->m_light_names ), std::end( getInstance()->m_light_names ), std::string() );
		getInstance()->m_uniform = {};
	}

	const sLight& cLightManager::get( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		const std::string*   names   = getInstance()->m_light_names;
		const sLightUniform& uniform = getInstance()->m_uniform;

		for( int i = 0; i < 64; ++i )
		{
			if( names[ i ] == _name )
				return uniform.lights[ i ];
		}

		return uniform.lights[ 63 ];
	}
}
