#include "cFontManager.h"

namespace df
{
	cFontManager::cFontManager()
	{
		DF_ProfilingScopeCpu;

		create( "roboto", "fonts/roboto/static/Roboto-Regular.ttf" );
	}

	cFont& cFontManager::create( const std::string& _name, const std::string& _file_path )
	{
		DF_ProfilingScopeCpu;

		cFont& font = getInstance()->m_fonts[ _name ];
		font.loadFromFile( _file_path );

		return font;
	}

	bool cFontManager::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		getInstance()->m_fonts.erase( _name );
		return true;
	}

	void cFontManager::clear()
	{
		DF_ProfilingScopeCpu;

		getInstance()->m_fonts.clear();
	}

	const cFont& cFontManager::get( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		return getInstance()->m_fonts.at( _name );
	}
}
