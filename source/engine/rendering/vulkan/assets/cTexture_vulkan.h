#pragma once

#include <string>

#include "engine/misc/Misc.h"
#include "engine/rendering/assets/iTexture.h"

namespace df::vulkan
{
	class cTexture_vulkan : public iTexture
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cTexture_vulkan );

		explicit cTexture_vulkan( std::string _name, int _target );
		~cTexture_vulkan() override;

		bool load( const std::string& _file, bool _flip_vertically_on_load = true, int _mipmaps = 0, bool _generate_mipmaps = true ) override;

		void setTexImage2D( int _level, int _internal_format, int _width, int _height, int _border, unsigned _format, unsigned _type, const void* _pixels ) const;
		void setTextureParameterI( int _name, int _param ) const;
		void setPixelStoreI( int _name, int _param ) const;

		void bind( int _index = 0 ) override;
		void unbind( int _index = 0 ) override;

		unsigned getTexture() const { return m_texture; }

	protected:
		unsigned    m_texture;
		int         m_target;
		std::string m_path;
	};
}
