﻿#pragma once

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
		static const cFont& get( std::uint16_t _id );

	private:
		std::uint16_t                                    m_next_id;
		std::unordered_map< std::string, std::uint16_t > m_name_id;
		std::unordered_map< std::uint16_t, cFont >       m_fonts;
	};
}
