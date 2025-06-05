#pragma once

#include <unordered_map>

#include "engine/core/utils/iSingleton.h"
#include "engine/graphics/assets/cFont.h"

namespace df
{
	class cFontManager : public iSingleton< cFontManager >
	{
	public:
		DF_DeleteCopyAndMove( cFontManager );

		cFontManager();
		~cFontManager() override = default;

		static cFont& create( const std::string& _name, const std::string& _file_path );

		static bool destroy( const std::string& _name );
		static void clear();

		static const cFont& get( const std::string& _name );

	private:
		std::unordered_map< std::string, cFont > m_fonts;
	};
}
