#pragma once

#include <string>

#include "engine/misc/Misc.h"
#include "engine/rendering/iFramebuffer.h"
#include "math/cVector.h"

namespace df::opengl
{
	class cFramebuffer_opengl : public iFramebuffer
	{
	public:
		DF_DisableCopyAndMove( cFramebuffer_opengl );

		explicit cFramebuffer_opengl( std::string _name, unsigned _num_render_textures = 0, bool _generate_render_buffer = true, const cVector2i& _size = cVector2i( -1, -1 ) );
		~cFramebuffer_opengl() override;

		static void setFramebufferTexture2D( int _attachment, int _tex_target, int _texture, int _level );

		void bind() override;
		void unbind() override;

	private:
		unsigned m_buffer;
		unsigned m_render_buffer;
	};
}
