#include "cFont.h"

#include <glad/glad.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include "engine/core/cFileSystem.h"
#include "engine/core/math/cVector.h"
#include "engine/profiling/ProfilingMacros.h"
#include "textures/cTexture2D.h"

namespace df
{

	cFont::cFont( const std::string& _name )
		: m_texture( cTexture2D::create( _name ) )
		, m_font_geometry( &m_glyphs )
	{}

	void cFont::loadFromFile( const std::string& _file )
	{
		DF_ProfilingScopeCpu;

		msdfgen::FreetypeHandle* freetype_handle = msdfgen::initializeFreetype();
		if( !freetype_handle )
			return;

		std::string          full_path   = cFileSystem::getPath( _file );
		msdfgen::FontHandle* font_handle = msdfgen::loadFont( freetype_handle, full_path.data() );

		if( !font_handle )
		{
			msdfgen::deinitializeFreetype( freetype_handle );
			return;
		}

		m_font_geometry.loadCharset( font_handle, 1, msdf_atlas::Charset::ASCII );

		for( msdf_atlas::GlyphGeometry& glyph: m_glyphs )
			glyph.edgeColoring( &msdfgen::edgeColoringInkTrap, 3, 0 );

		msdf_atlas::TightAtlasPacker packer;
		packer.setDimensionsConstraint( msdf_atlas::DimensionsConstraint::SQUARE );
		packer.setScale( 40 );
		packer.setPixelRange( 2 );
		packer.setMiterLimit( 1 );
		packer.pack( m_glyphs.data(), static_cast< int >( m_glyphs.size() ) );

		cVector2i size;
		packer.getDimensions( size.width(), size.height() );

		msdf_atlas::GeneratorAttributes attributes;

		msdf_atlas::ImmediateAtlasGenerator< float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage< msdf_atlas::byte, 3 > > generator( size.width(), size.height() );
		generator.setAttributes( attributes );
		generator.setThreadCount( 8 );
		generator.generate( m_glyphs.data(), static_cast< int >( m_glyphs.size() ) );

		msdfgen::BitmapConstRef< msdf_atlas::byte, 3 > atlas_storage = static_cast< msdfgen::BitmapConstRef< msdf_atlas::byte, 3 > >( generator.atlasStorage() );

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		m_texture->loadFromData( full_path, atlas_storage.pixels, size, true );
	}
}