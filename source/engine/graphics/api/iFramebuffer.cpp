#include "iFramebuffer.h"

#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iFramebuffer::~iFramebuffer()
	{
		DF_ProfilingScopeCpu;

		for( const cTexture2D* render_texture: m_render_textures )
			delete render_texture;
	}
}
