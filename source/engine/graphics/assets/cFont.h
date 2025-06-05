#pragma once

#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>
#include <string>
#include <vector>

#include "engine/core/utils/Misc.h"

namespace df
{
	class cTexture2D;

	class cFont
	{
	public:
		DF_DeleteCopyAndMove( cFont );

		cFont( const std::string& _name );
		~cFont();

		void loadFromFile( const std::string& _file );

		cTexture2D* getTexture() const { return m_texture; }

	private:
		cTexture2D* m_texture;

		std::vector< msdf_atlas::GlyphGeometry > m_glyphs;
		msdf_atlas::FontGeometry                 m_font_geometry;
	};
}
