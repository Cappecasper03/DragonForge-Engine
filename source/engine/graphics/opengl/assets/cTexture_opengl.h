#pragma once

#include <glad/glad.h>
#include <string>

#include "engine/core/math/cVector.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/iTexture.h"

namespace df::opengl
{
	struct sTextureParameter;
	struct sTextureImage;

	class cTexture_opengl final : public iTexture
	{
		friend sTextureParameter;
		friend sTextureImage;

	public:
		DF_DeleteCopyAndMove( cTexture_opengl );

		enum eType
		{
			k2D               = GL_TEXTURE_2D,
			kProxy2D          = GL_PROXY_TEXTURE_2D,
			kArray1D          = GL_TEXTURE_1D_ARRAY,
			kProxyArray1D     = GL_PROXY_TEXTURE_1D_ARRAY,
			kRectangle        = GL_TEXTURE_RECTANGLE,
			kProxyRectangle   = GL_PROXY_TEXTURE_RECTANGLE,
			kCubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			kCubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			kCubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			kCubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			kCubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			kCubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			kProxyCubeMap     = GL_PROXY_TEXTURE_CUBE_MAP,
		};

		explicit cTexture_opengl( std::string _name, eType _target );
		~cTexture_opengl() override;

		bool load( const std::string& _file, bool _mipmapped = false, int _mipmaps = 0, bool _flip_vertically_on_load = true ) override;

		void set2D( int _level, int _internal_format, const cVector2i& _size, int _border, int _format, unsigned _type, const void* _pixels ) const;

		void setInteger( int _name, int _param ) const;
		void setInteger( int _name, int _param[ 4 ] ) const;

		void bind( int _index = 0 ) override;
		void unbind( int _index = 0 ) override;

		unsigned get() const { return m_id; }

	protected:
		unsigned m_id;
		eType    m_type;
	};
}
