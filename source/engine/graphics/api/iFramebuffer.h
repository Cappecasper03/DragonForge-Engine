#pragma once

#include <vector>

#include "engine/core/utils/Misc.h"

namespace df
{
	class cTexture2D;

	class iFramebuffer
	{
	public:
		DF_DeleteCopyAndMove( iFramebuffer );

		iFramebuffer() = default;
		virtual ~iFramebuffer();

		virtual void bind() {}
		virtual void unbind() {}

		std::vector< cTexture2D* > m_render_textures;
	};
}
