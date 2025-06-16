#pragma once

#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>
#include <string>
#include <vector>

#include "engine/core/utils/cSmartPointers.h"
#include "engine/core/utils/Misc.h"
#include "textures/cTexture2D.h"

namespace df
{
	class cFont
	{
	public:
		DF_DeleteCopyAndMove( cFont );

		cFont();
		~cFont() = default;

		void loadFromFile( const std::string& _file );

		cTexture2D* getTexture() const { return m_texture.get(); }

		const msdf_atlas::FontGeometry& getGeometry() const { return m_geometry; }

		std::uint16_t getId() const { return m_id; }
		void          setId( const std::uint16_t _id ) { m_id = _id; }

	private:
		std::uint16_t         m_id;
		cUnique< cTexture2D > m_texture;

		std::vector< msdf_atlas::GlyphGeometry > m_glyphs;
		msdf_atlas::FontGeometry                 m_geometry;
	};
}
