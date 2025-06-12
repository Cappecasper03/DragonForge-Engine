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

		cFont();
		~cFont();

		void loadFromFile( const std::string& _file );

		cTexture2D* getTexture() const { return m_texture; }

		const msdf_atlas::FontGeometry& getGeometry() const { return m_geometry; }

		std::uint16_t getId() const { return m_id; }
		void          setId( const std::uint16_t _id ) { m_id = _id; }

	private:
		std::uint16_t m_id;
		cTexture2D*   m_texture;

		std::vector< msdf_atlas::GlyphGeometry > m_glyphs;
		msdf_atlas::FontGeometry                 m_geometry;
	};
}
