#pragma once

#include <vector>

#include "engine/core/utils/Misc.h"

namespace df
{
	class iTexture;

	class iFramebuffer
	{
	public:
		DF_DeleteCopyAndMove( iFramebuffer );

		iFramebuffer() = default;
		virtual ~iFramebuffer();

		virtual void bind() {}
		virtual void unbind() {}

		std::vector< iTexture* > m_render_textures;
	};
}
