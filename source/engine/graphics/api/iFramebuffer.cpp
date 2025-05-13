#include "iFramebuffer.h"

#include "engine/graphics/assets/iTexture.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iFramebuffer::iFramebuffer( std::string _name )
		: m_name( std::move( _name ) )
	{}

	iFramebuffer::~iFramebuffer()
	{
		DF_ProfilingScopeCpu;

		for( const iTexture* render_texture: m_render_textures )
			delete render_texture;
	}
}
