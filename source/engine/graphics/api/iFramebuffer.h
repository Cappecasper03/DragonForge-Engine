#pragma once

#include <string>
#include <vector>

#include "engine/core/utils/Misc.h"

namespace df
{
	class iTexture;

	class iFramebuffer
	{
	public:
		DF_DisableCopyAndMove( iFramebuffer );

		explicit iFramebuffer( std::string _name );
		virtual ~iFramebuffer();

		virtual void bind() {}
		virtual void unbind() {}

		std::vector< iTexture* > m_render_textures;
		const std::string        m_name;
	};
}
