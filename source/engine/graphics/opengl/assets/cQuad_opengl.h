#pragma once

#include <string>

#include "engine/misc/cColor.h"
#include "graphics/assets/iQuad.h"
#include "sRenderAsset_opengl.h"

namespace df::opengl
{
	class cTexture_opengl;

	class cQuad_opengl : public sRenderAsset_opengl,
						 public iQuad
	{
	public:
		DF_DisableCopyAndMove( cQuad_opengl );

		cQuad_opengl( std::string _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color = color::white );
		~cQuad_opengl() override = default;

		bool loadTexture( const std::string& _file_path, bool _mipmapped, int _mipmaps, bool _flip_vertically_on_load ) override;

		void render() override;

		static iRenderCallback* createDefaults();

	protected:
		struct sVertex
		{
			cVector3f position;
			cVector2f tex_coord;
		};
	};
}
