#pragma once

#include <string>

#include "engine/core/math/cVector.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/types/sTextureFormat.h"
#include "engine/graphics/types/sTextureParameter.h"

namespace df::opengl
{
	class cTexture2D_opengl final : public cTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D_opengl );

		cTexture2D_opengl( const std::string& _name );
		~cTexture2D_opengl() override;

		bool loadFromData( const std::string& _file, const void* _data, const cVector2i& _size, bool _mipmapped, int _mipmaps ) override;

		void set2D( int                     _level,
		            sTextureFormat::eFormat _internal_format,
		            const cVector2i&        _size,
		            int                     _border,
		            sTextureFormat::eFormat _format,
		            unsigned                _type,
		            const void*             _pixels ) const;

		void setInteger( sTextureParameter::eName _name, sTextureParameter::eParameter _param ) const;
		void setInteger( sTextureParameter::eName _name, const sTextureParameter::eParameter _param[ 4 ] ) const;

		void bind( int _index = 0 ) const override;
		void unbind( int _index = 0 ) const override;

		unsigned get() const { return m_id; }

	protected:
		unsigned   m_id;
		static int s_type;
	};
}
