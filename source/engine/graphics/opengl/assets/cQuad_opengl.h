#pragma once

#include <string>

#include "engine/core/utils/cColor.h"
#include "engine/graphics/assets/iQuad.h"
#include "sAsset_opengl.h"

namespace df::opengl
{
	class cTexture_opengl;

	class cQuad_opengl : public sAsset_opengl,
						 public iQuad
	{
	public:
		DF_DeleteCopyAndMove( cQuad_opengl );

		cQuad_opengl( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color = color::white );
		~cQuad_opengl() override = default;

		bool loadTexture( const std::string& _file_path, bool _mipmapped, int _mipmaps, bool _flip_vertically_on_load ) override;

		void render() override;

		static iRenderCallback* createDefaults();

		cBuffer_opengl m_push_constant;
	};
}
