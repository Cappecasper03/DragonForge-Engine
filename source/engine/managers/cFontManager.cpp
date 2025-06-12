#include "cFontManager.h"

#include <fmt/format.h>

namespace df
{
	cFontManager::cFontManager()
		: m_next_id( 1 )
	{
		DF_ProfilingScopeCpu;

		create( "roboto", "fonts/roboto/static/Roboto-Regular.ttf" );
	}

	cFont& cFontManager::create( const std::string& _name, const std::string& _file_path )
	{
		DF_ProfilingScopeCpu;

		cFontManager* instance = getInstance();

		if( instance->m_name_id.contains( _name ) )
		{
			DF_LogWarning( fmt::format( "Font already exist: {}", _name ) );
			return instance->m_fonts.at( instance->m_name_id.at( _name ) );
		}

		std::uint16_t& id = instance->m_name_id[ _name ];
		id                = instance->m_next_id++;

		cFont& font = instance->m_fonts[ id ];
		font.setId( id );
		font.loadFromFile( _file_path );

		DF_LogMessage( fmt::format( "Created font: {}", _name ) );
		return font;
	}

	bool cFontManager::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		cFontManager* instance = getInstance();

		if( !instance->m_name_id.contains( _name ) )
		{
			DF_LogWarning( fmt::format( "Font doesn't exist: {}", _name ) );
			return false;
		}

		instance->m_fonts.erase( instance->m_name_id.at( _name ) );
		instance->m_name_id.erase( _name );
		return true;
	}

	void cFontManager::clear()
	{
		DF_ProfilingScopeCpu;

		cFontManager* instance = getInstance();

		instance->m_name_id.clear();
		instance->m_fonts.clear();
		instance->m_next_id = 0;
	}

	const cFont& cFontManager::get( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		cFontManager* instance = getInstance();

		return instance->m_fonts.at( instance->m_name_id.at( _name ) );
	}

	const cFont& cFontManager::get( const std::uint16_t _id )
	{
		DF_ProfilingScopeCpu;

		cFontManager* instance = getInstance();

		return instance->m_fonts.at( _id );
	}
}
