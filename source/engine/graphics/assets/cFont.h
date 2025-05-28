#pragma once

#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>
#include <string>
#include <vector>

#include "cTexture.h"
#include "engine/core/utils/Misc.h"

namespace df
{

	class cFont
	{
	public:
		DF_DeleteCopyAndMove( cFont );

		cFont( const std::string& _name );
		~cFont() = default;

		void loadFromFile( const std::string& _file );

		iTexture* getTexture() const { return m_texture.get(); }

	private:
		cTexture m_texture;

		std::vector< msdf_atlas::GlyphGeometry > m_glyphs;
		msdf_atlas::FontGeometry                 m_font_geometry;
	};
}
