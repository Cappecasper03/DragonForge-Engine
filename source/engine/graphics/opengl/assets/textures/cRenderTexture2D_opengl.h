#pragma once

#include "engine/graphics/assets/textures/cRenderTexture2D.h"

namespace df::opengl
{
	class cRenderTexture2D_opengl : public cRenderTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cRenderTexture2D_opengl );

		cRenderTexture2D_opengl();
		~cRenderTexture2D_opengl() override;

		void clear( const cColor& _color ) override;

		void bind( int _index = 0 ) const override;
		void unbind( int _index = 0 ) const override;

		unsigned get() const { return m_id; }

	protected:
		void initialize( const sDescription& _description ) override;

	protected:
		unsigned m_id;
	};
}
