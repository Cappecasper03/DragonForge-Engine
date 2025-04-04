#include "iFramebuffer.h"

#include "engine/profiling/ProfilingMacros.h"
#include "assets/iTexture.h"

namespace df
{
	iFramebuffer::iFramebuffer( std::string _name )
		: name( std::move( _name ) )
	{}

	iFramebuffer::~iFramebuffer()
	{
		DF_ProfilingScopeCPU;

		for( const iTexture* render_textue: render_textues )
			delete render_textue;
	}
}
