﻿#pragma once

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/types/sTextureFormat.h"

namespace df::opengl
{
	class cTexture2D_opengl final : public cTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D_opengl );

		cTexture2D_opengl();
		~cTexture2D_opengl() override;

		void clear( const cColor& _color ) override;

		void uploadData( const void* _data, sTextureFormat::eFormat _format, unsigned _mip_level = 0, bool _generate_mipmap = true ) override;

		void bind( int _index = 0 ) const override;
		void unbind( int _index = 0 ) const override;

		unsigned get() const { return m_id; }

	protected:
		void initialize( const sDescription& _description ) override;

		unsigned m_id;
	};
}
