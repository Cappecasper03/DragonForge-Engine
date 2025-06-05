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
		: m_texture( nullptr )
		, m_font_geometry( &m_glyphs )
	{}

	cFont::~cFont()
	{
		DF_ProfilingScopeCpu;

		delete m_texture;
	}

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
		packer.setInnerPixelPadding( 0 );
		packer.setOuterPixelPadding( 0 );
		packer.pack( m_glyphs.data(), static_cast< int >( m_glyphs.size() ) );

		cVector2i size;
		packer.getDimensions( size.width(), size.height() );

		msdf_atlas::ImmediateAtlasGenerator< float, 4, msdf_atlas::mtsdfGenerator, msdf_atlas::BitmapAtlasStorage< msdf_atlas::byte, 4 > > generator( size.width(), size.height() );
		generator.setThreadCount( 8 );
		generator.generate( m_glyphs.data(), static_cast< int >( m_glyphs.size() ) );

		msdfgen::BitmapConstRef< msdf_atlas::byte, 4 > atlas_storage = static_cast< msdfgen::BitmapConstRef< msdf_atlas::byte, 4 > >( generator.atlasStorage() );

		cTexture2D::sDescription description{
			.name       = "font",
			.size       = size,
			.mip_levels = 1,
			.format     = sTextureFormat::kRGBA,
			.usage      = sTextureUsage::kSampled | sTextureUsage::kTransferDestination,
		};
		m_texture = cTexture2D::create( description );
		m_texture->uploadData( atlas_storage.pixels, sTextureFormat::kRGBA );
	}
}