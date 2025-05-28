#pragma once

#include <string>

#include "engine/core/math/cVector.h"
#include "engine/core/utils/Misc.h"

namespace df
{
	class iTexture
	{
	public:
		DF_DeleteCopyAndMove( iTexture );

		explicit iTexture( std::string _name );
		virtual ~iTexture() = default;

		bool         load( const std::string& _file, bool _mipmapped = false, int _mipmaps = 0, bool _flip_vertically_on_load = true );
		virtual bool loadFromData( const std::string& _file, const void* _data, const cVector2i& _size, bool _mipmapped = false, int _mipmaps = 0 ) = 0;

		virtual void bind( int /*_index*/ = 0 ) const   = 0;
		virtual void unbind( int /*_index*/ = 0 ) const = 0;

		const cVector2f& getSize() const { return m_size; }

		std::string m_name;

	protected:
		cVector2f   m_size;
		std::string m_file_path;
	};
}
