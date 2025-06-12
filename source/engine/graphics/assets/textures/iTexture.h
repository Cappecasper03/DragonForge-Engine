#pragma once

#include "engine/core/math/cVector.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/types/sTextureFormat.h"

namespace df
{
	class iTexture
	{
	public:
		DF_DeleteCopyAndMove( iTexture );

		iTexture()          = default;
		virtual ~iTexture() = default;

		virtual void bind( int /*_index*/ = 0 ) const   = 0;
		virtual void unbind( int /*_index*/ = 0 ) const = 0;

		virtual const cVector2u&               getSize() const   = 0;
		virtual const sTextureFormat::eFormat& getFormat() const = 0;
	};
}
