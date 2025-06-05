#include "cLightManager.h"

#include <fmt/format.h>

namespace df
{
	sLight& cLightManager::create( const std::string& _name, const sLight& _light )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, unsigned >& name_index = getInstance()->m_name_index;
		std::unordered_map< unsigned, std::string >& index_name = getInstance()->m_index_name;
		std::vector< sLight >&                       lights     = getInstance()->m_lights;

		if( lights.size() >= m_max_lights )
		{
			DF_LogWarning( fmt::format( "Can't add more lights( {} )", m_max_lights ) );
			return lights.back();
		}

		if( name_index.contains( _name ) )
		{
			DF_LogWarning( fmt::format( "Light already exist: {}", _name ) );
			return lights[ name_index[ _name ] ];
		}

		const unsigned index = static_cast< unsigned >( lights.size() );
		name_index[ _name ]  = index;
		index_name[ index ]  = _name;
		lights.push_back( _light );
		DF_LogMessage( fmt::format( "Created light: {}", _name ) );
		return lights.back();
	}

	bool cLightManager::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, unsigned >& name_index = getInstance()->m_name_index;
		std::unordered_map< unsigned, std::string >& index_name = getInstance()->m_index_name;
		std::vector< sLight >&                       lights     = getInstance()->m_lights;

		const auto it = name_index.find( _name );
		if( it == name_index.end() )
		{
			DF_LogWarning( fmt::format( "Light doesn't exist: {}", _name ) );
			return false;
		}

		const unsigned index      = it->second;
		const unsigned last_index = static_cast< unsigned >( lights.size() - 1 );

		if( index != last_index )
		{
			std::swap( lights[ index ], lights[ last_index ] );
			name_index[ index_name[ last_index ] ] = index;
			index_name[ index ]                    = index_name[ last_index ];
		}

		name_index.erase( it );
		index_name.erase( last_index );
		lights.pop_back();

		DF_LogMessage( fmt::format( "Destroyed light: {}", _name ) );
		return true;
	}

	void cLightManager::clear()
	{
		DF_ProfilingScopeCpu;

		getInstance()->m_name_index.clear();
		getInstance()->m_lights.clear();
	}

	const sLight& cLightManager::get( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		const std::unordered_map< std::string, unsigned >& name_index = getInstance()->m_name_index;
		const std::vector< sLight >&                       lights     = getInstance()->m_lights;

		return lights[ name_index.at( _name ) ];
	}
}
